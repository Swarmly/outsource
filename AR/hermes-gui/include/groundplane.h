#pragma once

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>

class GroundPlane : public Qt3DCore::QEntity
{
public:
    GroundPlane(const QVector3D &position, Qt3DCore::QEntity *parent = nullptr);

    Qt3DCore::QTransform *transform() { return mTransform; }
    void setMaterial(Qt3DRender::QMaterial *material);

private:
    Qt3DRender::QMaterial *mMaterial = nullptr;
    Qt3DCore::QTransform *mTransform = nullptr;
};
