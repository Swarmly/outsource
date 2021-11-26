#pragma once

#include <QCamera>
#include <QVector3D>
#include <Qt3DCore/QEntity>

#include "toyplane.h"

class FollowEntityCamera : public Qt3DRender::QCamera
{
public:
    FollowEntityCamera(QCamera *camera = nullptr, ToyPlane *entity = nullptr);
    virtual ~FollowEntityCamera();

    void update(const QVector3D &position, float roll, float pitch, float yaw);

    void set_distance_from_aircraft(float distance);
    void set_angle_around_aircraft(float angle);

private:
    QVector3D camera_offset_ = QVector3D(0, 0.0f, -20.0f);

    QCamera *camera_entity_ = nullptr;
    ToyPlane *airplane_ = nullptr;
    float pitch_, yaw_, roll_;
    float distance_from_airplane_;
    float angle_around_airplane_;

    float rotation_y_ = 0;
};
