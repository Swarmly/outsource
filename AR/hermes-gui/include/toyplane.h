#pragma once

#include <Qt3DCore>
#include <Qt3DCore/QComponent>
#include <Qt3DExtras>
#include <Qt3DRender>

class ToyPlaneTransform : public Qt3DCore::QTransform {
public:
    ToyPlaneTransform(Qt3DCore::QComponent *parent = nullptr);

    QMatrix4x4 update_angles(const QVector3D &position, float roll, float pitch, float yaw);
    //    void set_scale_factor(float scale_factor) { this->scale_factor = scale_factor; }

private:
    float roll = 0;
    float pitch = 0;
//    float altitude = 0;
    float yaw = 0;
//    float scale_factor = 0.1;
};

class ToyPlane : public Qt3DCore::QEntity
{
public:
    ToyPlane(const QVector3D &position, Qt3DCore::QEntity *parent = nullptr);

    void setMaterial(Qt3DRender::QMaterial *material);

    ToyPlaneTransform* transform() { return transform_; }
    QVector3D get_position();
    QVector3D euler_angles();
private:
    Qt3DRender::QMaterial *material_ = nullptr;
    ToyPlaneTransform *transform_ = nullptr;
};
