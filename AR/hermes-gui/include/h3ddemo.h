#pragma once
#include "hermes.h"
#include <QtCore/QTimer>
#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>
#include <QtWidgets/QWidget>
#include <memory>
#include "baseguiwidget.h"

namespace Ui {
class H3DDemo;
}

namespace Qt3DExtras {
class Qt3DWindow;
class QFirstPersonCameraController;
class QOrbitCameraController;
} // namespace Qt3DExtras

namespace Qt3DInput {
class QInputAspect;
}

namespace Qt3DRender {
class QCamera;
class QPointLight;
} // namespace Qt3DRender

namespace Qt3DCore {
class QEntity;
class QTransform;
} // namespace Qt3DCore

class SceneModifier;
class FollowEntityCamera;

class H3DDemo  : public BaseGuiWidget
{
    Q_OBJECT

public:
    explicit H3DDemo(QWidget *parent = nullptr);
    ~H3DDemo();

private:
    void camera_update();

private slots:
    void updateCamPos();
    void predictData();
    void onMavlinkListSignal(std::list<mavlink_message_t> outMessages);
    void on_apply_button_clicked();


private:
    QTimer m_predictTimer;
    Ui::H3DDemo *ui;

    QVector3D last_pos;
    mavlink_local_position_ned_t local_position_ned_msg;
    mavlink_ahrs2_t ahrs2_msg;
    mavlink_attitude_t attitude;

    float dist_scale_factor = 0.1;

    float vehicle_roll_ = 0; /*< [degrees] Roll angle.*/
    float vehicle_pitch_ = 0; /*< [degrees] Pitch angle.*/
    float vehicle_yaw_ = 0; /*< [degrees] Yaw angle.*/

    float camera_roll_ = 0; /*< [degrees] Roll angle.*/
    float camera_pitch_ = 0; /*< [degrees] Pitch angle.*/

    QVector3D camera_offset_ = QVector3D(0, 0.0f, 20.0f);
    QVector3D vehicle_offset_ = QVector3D(0, 0.0f, -20.0f);
    QVector3D camera_angles_;
    QMatrix4x4 angle4D;

    std::unique_ptr<FollowEntityCamera> follow_camera_;

    Qt3DExtras::Qt3DWindow *view;
    Qt3DInput::QInputAspect *input;
    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *cameraEntity;
    Qt3DCore::QEntity *lightEntity;
    Qt3DRender::QPointLight *light;
    Qt3DCore::QTransform *lightTransform;
    Qt3DExtras::QFirstPersonCameraController *camController;
    SceneModifier *modifier;
    QTimer timer;
    QMutex m_guard;
};
