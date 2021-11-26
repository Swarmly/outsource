#include "groundplane.h"

GroundPlane::GroundPlane(const QVector3D &position, Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent)
{
    auto *plane = new Qt3DExtras::QPlaneMesh;
    plane->setWidth(100.0f);
    plane->setHeight(100.0f);
    plane->setMeshResolution(QSize(2, 2));

    mTransform = new Qt3DCore::QTransform;
    mTransform->setTranslation(position);

    addComponent(plane);
    addComponent(mTransform);
}

void GroundPlane::setMaterial(Qt3DRender::QMaterial *material)
{
    mMaterial = material;
    addComponent(material);
}
