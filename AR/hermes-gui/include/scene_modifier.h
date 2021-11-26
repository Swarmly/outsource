#pragma once

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
#include <QtCore/QTimer>

#include "toyplane.h"

class SceneModifier : public QObject
{
    Q_OBJECT
    enum Shape { Torus, Cone, Cylinder, Cuboid, Plane, Sphere };

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity);
    ~SceneModifier() = default;

    ToyPlane* toyplane() { return toy_plane; }

public slots:
    void enableTorus(bool enabled);
    void enableCone(bool enabled);
    void enableCylinder(bool enabled);
    void enableCuboid(bool enabled);
    void enablePlane(bool enabled);
    void enableSphere(bool enabled);

private:
    void add_ground_plane(const QVector3D &pos);
    void add_torus(const QVector3D &pos);
    void add_cone(const QVector3D &pos);
    void add_cylinder(const QVector3D &pos);
    void add_cuboid(const QVector3D &pos);
    void add_plane(const QVector3D &pos);
    void add_sphere(const QVector3D &pos);
    void add_toy_plane(const QVector3D& pos);

    void add_shape(const Shape &shape, const QVector3D &pos);
private:
    Qt3DCore::QEntity *m_rootEntity;
    QList<Qt3DCore::QEntity*> cone;
    QList<Qt3DCore::QEntity*> cylinder;
    QList<Qt3DCore::QEntity*> torus;
    QList<Qt3DCore::QEntity *> cuboid;
    QList<Qt3DCore::QEntity *> plane;
    QList<Qt3DCore::QEntity *> sphere;

    ToyPlane* toy_plane = nullptr;

    const int scene_dist = 800;

    QTimer timer;
};
