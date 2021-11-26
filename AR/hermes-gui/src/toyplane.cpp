#include "toyplane.h"

ToyPlaneTransform::ToyPlaneTransform(Qt3DCore::QComponent *parent)
    : Qt3DCore::QTransform(parent)
{}

QMatrix4x4 ToyPlaneTransform::update_angles(const QVector3D &position,
                                            float roll,
                                            float pitch,
                                            float yaw)
{
    QMatrix4x4 m;
    m.translate(position);
    m.scale(scale());
    m.rotate(yaw, QVector3D(0, 1, 0));
    m.rotate(pitch, QVector3D(0, 0, 1));
    m.rotate(roll, QVector3D(1, 0, 0));
    setMatrix(m);
}

ToyPlane::ToyPlane(const QVector3D &position, Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent)
{
    auto *mesh = new Qt3DRender::QMesh(this);
    mesh->setSource(QUrl("qrc:/3d/toyplane.obj"));

    transform_ = new ToyPlaneTransform();
    transform_->setScale(0.25f);
    transform_->update_angles(position, 0.0f, 0.0f, 0.0f);

    addComponent(mesh);
    addComponent(transform_);
}

void ToyPlane::setMaterial(Qt3DRender::QMaterial *material)
{
    this->material_ = material;
    addComponent(material);
}

QVector3D ToyPlane::get_position() {
    return !transform_ ? QVector3D() : transform_->translation();
}

QVector3D ToyPlane::euler_angles() {
    return !transform_ ? QVector3D() : QVector3D(
               transform_->rotationX(),
               transform_->rotationY(),
               transform_->rotationZ()
   );
}

