#include "follow_entity_camera.h"

#include <Qt3DInput>
#include <QtMath>

FollowEntityCamera::FollowEntityCamera(QCamera *camera, ToyPlane *airplane)
    : camera_entity_(camera)
    , airplane_(airplane)
{
    //camera_pos_ = QVector3D(0.0f, 0.0f, 0.0f);

    camera_entity_->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    //camera_entity_->setPosition(camera_pos_);
    camera_entity_->setUpVector(QVector3D(0, 1, 0));
    camera_entity_->setViewCenter(airplane_->get_position());

    pitch_ = 0.0f;
    yaw_ = 0.0f;
    roll_ = 0.0f;
    distance_from_airplane_ = 10.0f;
    angle_around_airplane_ = 90;
}

void FollowEntityCamera::set_distance_from_aircraft(float distance) {
    distance_from_airplane_ = distance;
}

void FollowEntityCamera::set_angle_around_aircraft(float angle) {
    angle_around_airplane_ = angle;
}

FollowEntityCamera::~FollowEntityCamera()
{
    // Empty
}

void FollowEntityCamera::update(const QVector3D &position, float roll, float pitch, float yaw)
{
    QMatrix4x4 rotationAngle;
    rotationAngle.rotate(yaw, QVector3D(0, 1, 0));

    //    {\displaystyle \mathbf {q} _{0}=\mathbf {q} _{w}=\cos(\alpha /2)}
    //    {\displaystyle \mathbf {q} _{1}=\mathbf {q} _{x}=\sin(\alpha /2)\cos(\beta _{x})}{\displaystyle \mathbf {q} _{1}=\mathbf {q} _{x}=\sin(\alpha /2)\cos(\beta _{x})}
    //    {\displaystyle \mathbf {q} _{2}=\mathbf {q} _{y}=\sin(\alpha /2)\cos(\beta _{y})}{\displaystyle \mathbf {q} _{2}=\mathbf {q} _{y}=\sin(\alpha /2)\cos(\beta _{y})}
    //    {\displaystyle \mathbf {q} _{3}=\mathbf {q} _{z}=\sin(\alpha /2)\cos(\beta _{z})}{\displaystyle \mathbf {q} _{3}=\mathbf {q} _{z}=\sin(\alpha /2)\cos(\beta _{z})}

    // rotationAngle.rotate(vehicle_pitch_, QVector3D(0, 0, 1));
    // rotationAngle.rotate(vehicle_roll_, QVector3D(1, 0, 0));
    //    float hDist = distance_from_airplane_ * qCos(qDegreesToRadians(pitch));
    //    float vDist = distance_from_airplane_ * qSin(qDegreesToRadians(pitch));

    camera_entity_->setPosition(position);
    QVector3D viewCenter = position + camera_offset_ * rotationAngle;
    camera_entity_->setViewCenter(viewCenter);
    //    float theta = airplane_->euler_angles().y() + angle_around_airplane_;
    //    float offsetX = hDist * qSin(qDegreesToRadians(theta));
    //    float offsetZ = hDist * qCos(qDegreesToRadians(theta));

    //    camera_pos_.setX(airplane_pos_.x() - offsetX);
    //    camera_pos_.setY(airplane_pos_.y() - vDist);
    //    camera_pos_.setZ(airplane_pos_.z() - offsetZ);

    // unrotate
    //    camera_entity_->rotateAboutViewCenter(
    //        camera_entity_->rotation(-roll_, QVector3D(1, 0, 0)));

    //    camera_entity_->setPosition(camera_pos_);
    //    camera_entity_->setViewCenter(airplane_->get_position());

    ////    camera_entity_->rotateAboutViewCenter(
    ////        camera_entity_->rotation(yaw - yaw_, QVector3D(0, 1, 0)));
    ////    cameraEntity->rotateAboutViewCenter(
    ////        cameraEntity->rotation(-camera_angles_.y(), QVector3D(, 1, 0)));
    //    camera_entity_->rotateAboutViewCenter(
    //        camera_entity_->rotation(roll, QVector3D(1, 0, 0)));

    //    // update angles
    //    pitch_ = pitch;
    //    yaw_ = yaw;
    //    roll_ = roll;
}
