#include "h3ddemo.h"
#include "ui_h3ddemo.h"

#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>

#include <QtGui/QScreen>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

#include <Qt3DInput/QInputAspect>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qtexture.h>

#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qorbitcameracontroller.h>
#include <Qt3DExtras/qt3dwindow.h>

#include <QtMath>

#include "scene_modifier.h"
#include "follow_entity_camera.h"

H3DDemo::H3DDemo(QWidget *parent)
    : BaseGuiWidget(parent)
    , ui(new Ui::H3DDemo)
{
    ui->setupUi(this);

    view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMaximumSize(screenSize);
    container->setMinimumSize(QSize(200, 100));
    ui->mainLayout->addWidget(container, 1);

    input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    // Camera
    cameraEntity = view->camera();

    lightEntity = new Qt3DCore::QEntity(rootEntity);
    light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(0.5);
    lightEntity->addComponent(light);
    lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    QObject::connect(cameraEntity,
                     &Qt3DRender::QCamera::positionChanged,
                     lightTransform,
                     &Qt3DCore::QTransform::setTranslation);

    // For camera controls
    camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);
    camController->setLinearSpeed(100);

    // Scenemodifier
    modifier = new SceneModifier(rootEntity);

    follow_camera_ = std::make_unique<FollowEntityCamera>(cameraEntity, modifier->toyplane());

    // Set root object of the scene
    view->setRootEntity(rootEntity);

    //    QObject::connect(ui->torus, &QCheckBox::stateChanged, modifier, &SceneModifier::enableTorus);
    //    QObject::connect(ui->cone, &QCheckBox::stateChanged, modifier, &SceneModifier::enableCone);
    //    QObject::connect(ui->cylinder,
    //                     &QCheckBox::stateChanged,
    //                     modifier,
    //                     &SceneModifier::enableCylinder);
    //    QObject::connect(ui->cuboid, &QCheckBox::stateChanged, modifier, &SceneModifier::enableCuboid);
    //    QObject::connect(ui->plane, &QCheckBox::stateChanged, modifier, &SceneModifier::enablePlane);
    //    QObject::connect(ui->sphere, &QCheckBox::stateChanged, modifier, &SceneModifier::enableSphere);

//    LOCAL_ASSERT(connect(Hermes2::getImpl(),
//                         &Hermes2::onMavlinkListSignal,
//                         this,
//                         &H3DDemo::onMavlinkListSignal));

    //    timer.setInterval(2000);
    //    connect(&timer, &QTimer::timeout, [this]() {
    //        // this->cameraEntity->translate(QVector3D(0, 0, 10));
    //        // cameraEntity->pan(5, QVector3D(0, 1, 0));
    //    });
    //    timer.start();
    follow_camera_->update(QVector3D(0, 0, 0), 0, 0, 0);

    memset(&local_position_ned_msg, 0, sizeof(local_position_ned_msg));
    memset(&ahrs2_msg, 0, sizeof(ahrs2_msg));
    memset(&attitude, 0, sizeof(attitude));
    connect(&m_predictTimer, &QTimer::timeout, this, &H3DDemo::predictData);
    m_predictTimer.start(40);
}

H3DDemo::~H3DDemo()
{
    delete ui;
}

void H3DDemo::updateCamPos()
{
    // Camera:
    //
    // Y -> Up, X -> Right, z -> towards you
    //          Y
    //          |
    //          |_____ X
    //         /
    //        /
    //      Z

    // LOCAL_POSITION_NED: Coordinate frame is right-handed, Z-axis down (aeronautical frame, NED / north-east-down convention).
    // https://en.wikipedia.org/wiki/Axes_conventions
    //
    //           ______ X
    //          /|
    //         / |
    //        Y  Z
    //

    //    LOG_WARNING_EXT << "Vehicle position: x=" << local_position_ned_msg.x
    //                    << "; y=" << local_position_ned_msg.y << "; z=" << local_position_ned_msg.z;

    QMutexLocker locker(&m_guard);

    QVector3D new_pos(local_position_ned_msg.x, -local_position_ned_msg.z, local_position_ned_msg.y);

    double cy = qCos(qDegreesToRadians(vehicle_pitch_) * 0.5);
    double sy = qSin(qDegreesToRadians(vehicle_pitch_) * 0.5);
    double cp = qCos(qDegreesToRadians(vehicle_yaw_ - 90) * 0.5);
    double sp = qSin(qDegreesToRadians(vehicle_yaw_ - 90) * 0.5);
    double cr = qCos(qDegreesToRadians(vehicle_roll_) * 0.5);
    double sr = qSin(qDegreesToRadians(vehicle_roll_) * 0.5);

    //UAV
    QQuaternion quat(cr * cp * cy + sr * sp * sy,
                     sr * cp * cy - cr * sp * sy,
                     cr * sp * cy + sr * cp * sy,
                     cr * cp * sy - sr * sp * cy);
    QMatrix4x4 rotationAngle;
    rotationAngle.rotate(quat);

    QVector3D offsetUAV = rotationAngle * vehicle_offset_;
    auto vehicle_object_pos = new_pos + offsetUAV;

    modifier->toyplane()->transform()->update_angles(vehicle_object_pos,
                                                     vehicle_roll_,
                                                     vehicle_pitch_,
                                                     vehicle_yaw_);

    //Camera
    //            cp = qCos(qDegreesToRadians(vehicle_yaw_ - 90 /*- camera_roll_*/) * 0.5);
    //            sp = qSin(qDegreesToRadians(vehicle_yaw_ - 90 /*- camera_roll_*/) * 0.5);
    //            cr = qCos(qDegreesToRadians(vehicle_pitch_ /*+ camera_pitch_*/) * 0.5);
    //            sr = qSin(qDegreesToRadians(vehicle_pitch_ /*+ camera_pitch_*/) * 0.5);

    //            QQuaternion quatCam(cr * cp * cy + sr * sp * sy,
    //                                sr * cp * cy - cr * sp * sy,
    //                                cr * sp * cy + sr * cp * sy,
    //                                cr * cp * sy - sr * sp * cy);
    //            QMatrix4x4 rotationAngleCam;
    //            rotationAngle.rotate(quat);
    //            auto cameraViewPos = rotationAngleCam * vehicle_offset_;

    cameraEntity->setPosition(new_pos);
    cameraEntity->setViewCenter(vehicle_object_pos);
    Qt3DCore::QTransform *transformCamera = cameraEntity->transform();

    if (transformCamera->rotationZ() != -vehicle_roll_) {
        cameraEntity->roll(transformCamera->rotationZ() + vehicle_roll_);
    }

    last_pos = new_pos;
}

void H3DDemo::predictData()
{
    {
        QMutexLocker locker(&m_guard);

        static float devider = 25;
        static float devider3 = 25;
        static float devider2 = 1;
        static float devider4 = 0;
        local_position_ned_msg.vx *= devider2;
        local_position_ned_msg.vy *= devider2;
        local_position_ned_msg.vz *= devider2;

        local_position_ned_msg.x += local_position_ned_msg.vx / devider;
        local_position_ned_msg.y += local_position_ned_msg.vy / devider;
        local_position_ned_msg.z += local_position_ned_msg.vz / devider;

        attitude.rollspeed *= devider4;
        attitude.pitch *= devider4;
        attitude.yawspeed *= devider4;

        //    attitude.roll += attitude.rollspeed / devider3;
        attitude.pitch -= attitude.pitch / devider3;
        attitude.yaw += attitude.yawspeed / devider3;

        //    camera_pitch_ = pitch;
        //    camera_roll_ = roll;

        float yaw = qRadiansToDegrees(-attitude.yaw) - camera_roll_;
        float pitch = qRadiansToDegrees(attitude.pitch) + camera_pitch_;
        float roll = qRadiansToDegrees(attitude.roll);

        //LOG_WARNING_EXT << "Vehicle Angles: yaw=" << yaw << "; pitch=" << pitch << "; roll=" << roll;

        vehicle_yaw_ = yaw;
        vehicle_pitch_ = pitch;
        vehicle_roll_ = roll;
    }
    updateCamPos();
}

// /Users/oleksandrbazna/Downloads/gsc-recorded-data/videos_0020_Wed_Jun_9_13.25.29_2021.ts
void H3DDemo::onMavlinkListSignal(std::list<mavlink_message_t> outMessages)
{
    for (auto &message : outMessages) {
        switch (message.msgid) {
        case MAVLINK_MSG_ID_LOCAL_POSITION_NED: {
            mavlink_msg_local_position_ned_decode(&message, &local_position_ned_msg);
            // updateCamPos();

            break;
        }
        case MAVLINK_MSG_ID_ATTITUDE: {
            mavlink_msg_attitude_decode(&message, &attitude);
            break;
        }
        case MAVLINK_MSG_ID_AHRS2: {
            mavlink_msg_ahrs2_decode(&message, &ahrs2_msg);

            // Camera:
            // Tilt: Up -> Positive angle, Down -> negative angle
            // Pan: Right -> Positive angle, Left - negative angle

            //            float yaw = qRadiansToDegrees(-ahrs2_msg.yaw);
            //            float pitch = qRadiansToDegrees(ahrs2_msg.pitch);
            //            float roll = qRadiansToDegrees(ahrs2_msg.roll);

            //            LOG_WARNING_EXT << "Vehicle Angles: yaw=" << yaw << "; pitch=" << pitch
            //                            << "; roll=" << roll;

            //            vehicle_yaw_ = yaw;
            //            vehicle_pitch_ = pitch;
            //            vehicle_roll_ = roll;

            break;
        }
        case MAVLINK_MSG_ID_V2_EXTENSION: {
            /* 248 = V2_EXTENSION  message id */

            // take the message as GROUND_CROSSING_REPORT (the shortest one),
            // for extracting the report_type
            mavlink_nv_ext_ground_crossing_report_t gcr;
            mavlink_nv_ext_ground_crossing_report_decode(&message, &gcr);

            switch (gcr.report_type) {
            case NextVisionSystemReport: {
                mavlink_nv_ext_system_report_t system_report;
                mavlink_nv_ext_system_report_decode(&message, &system_report);

                // Telemetry:
                // Roll: Camera roll angle (deg, -180..+180), float
                // Pitch: Camera pitch angle (deg, -180..+180), float
                // FOV: Camera horizontal FOV in deg, float
                float pitch = system_report.pitch;
                float roll = system_report.roll;
                float fov = system_report.fov;

                LOG_WARNING_EXT << "Vehicle Camera Angles: pitch=" << pitch << "; roll=" << roll;
                LOG_WARNING_EXT << "Vehicle Camera FOV: fov=" << fov;

                cameraEntity->setFieldOfView(fov);

                camera_pitch_ = pitch;
                camera_roll_ = roll;
                break;
            }
            case NextVisionLineOfSightReport: {
                mavlink_nv_ext_los_report_t los_report;
                mavlink_nv_ext_los_report_decode(&message, &los_report);

                // TODO: Handle
                break;
            }

            case NextVisionGroundCrossingReport: {
                // TODO: Handle
                break;
            }
            }
            break;
        }
        }
    }
}

void H3DDemo::camera_update()
{
    //    follow_camera_->update(vehicle_roll_,
    //                           vehicle_pitch_ - camera_pitch_,
    //                           vehicle_yaw_ - camera_roll_);
}

void H3DDemo::on_apply_button_clicked()
{
    //    follow_camera_->set_distance_from_aircraft(ui->distance_from_airplane_edit->text().toFloat());
    //    follow_camera_->set_angle_around_aircraft(ui->angle_around_airplane_edit->text().toFloat());
    camera_update();
}
