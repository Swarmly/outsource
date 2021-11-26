#include "scene_modifier.h"

#include <QtCore/QDebug>
#include <QtCore/QEventLoop>
#include "groundplane.h"

#define scaleFactor 10.0

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
{
    Shape shapes[] = {Torus, Cone, Cylinder, Cuboid, Plane, Sphere};

    // generate space
    for (int x = -scene_dist; x < scene_dist; x += 150) {
        for (int z = -scene_dist; z < scene_dist; z += 150) {
            for (int y = -scene_dist; y < scene_dist; y += 250) {
                //                if (z == 0)
                //                    continue;
                add_shape(shapes[rand() % 6], QVector3D(x, y, z));
            }
        }
    }

    // generate space
    for (int x = -scene_dist * 4; x < scene_dist * 4; x += 500) {
        for (int z = -scene_dist * 4; z < scene_dist * 4; z += 500) {
            //            if (z == 0)
            //                continue;
            add_shape(shapes[rand() % 6], QVector3D(x, 150, z));
        }
    }

    // add ground plane
    add_ground_plane(QVector3D(0.0f, -1.0f, 0.0f));

    // add toy plane
    add_toy_plane(QVector3D(0.0f, 0.0f, -20.0f));
}

void SceneModifier::add_shape(const Shape &shape, const QVector3D &pos)
{
    switch (shape) {
    case Torus:
        add_torus(pos);
        break;
    case Cone:
        add_cone(pos);
        break;
    case Cylinder:
        add_cylinder(pos);
        break;
    case Cuboid:
        add_cuboid(pos);
        break;
    case Plane:
        add_plane(pos);
        break;
    case Sphere:
        add_sphere(pos);
        break;
    }
}

void SceneModifier::add_ground_plane(const QVector3D &pos) {
    auto *ground = new GroundPlane(pos, m_rootEntity);
    ground->setObjectName("ground");
    auto *groundMaterial = new Qt3DExtras::QPhongMaterial;
    groundMaterial->setAmbient(QColor(Qt::darkGreen));
    ground->setMaterial(groundMaterial);
}

void SceneModifier::add_toy_plane(const QVector3D& pos) {
    auto *toy = new ToyPlane(pos, m_rootEntity);
    toy->setObjectName("toyplane");
    auto *toyMaterial = new Qt3DExtras::QPhongMaterial;
    toyMaterial->setAmbient(QColor(Qt::red));
    toy->setMaterial(toyMaterial);
    toy_plane = toy;
}

void SceneModifier::add_torus(const QVector3D &pos)
{
    // Torus shape data
    //! [0]
    auto torus = new Qt3DExtras::QTorusMesh();
    torus->setRadius(1.0f);
    torus->setMinorRadius(0.4f);
    torus->setRings(100);
    torus->setSlices(20);
    //! [0]

    // TorusMesh Transform
    //! [1]
    auto *torusTransform = new Qt3DCore::QTransform();
    torusTransform->setScale(scaleFactor);
    //    torusTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
    torusTransform->setTranslation(pos);
    //! [1]

    //! [2]
    auto *torusMaterial = new Qt3DExtras::QPhongMaterial();
    torusMaterial->setDiffuse(Qt::blue);
    //! [2]

    // Torus
    //! [3]
    auto torusEntity = new Qt3DCore::QEntity(m_rootEntity);
    torusEntity->addComponent(torus);
    torusEntity->addComponent(torusMaterial);
    torusEntity->addComponent(torusTransform);
    //! [3]

    this->torus.append(torusEntity);
}

void SceneModifier::add_cone(const QVector3D &pos)
{
    auto *cone = new Qt3DExtras::QConeMesh();
    cone->setTopRadius(0.5);
    cone->setBottomRadius(1);
    cone->setLength(3);
    cone->setRings(50);
    cone->setSlices(20);

    // ConeMesh Transform
    auto *coneTransform = new Qt3DCore::QTransform();
    coneTransform->setScale(scaleFactor);
    //    coneTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));
    coneTransform->setTranslation(pos);

    auto *coneMaterial = new Qt3DExtras::QPhongMaterial();
    coneMaterial->setDiffuse(Qt::darkCyan);

    // Cone
    auto coneEntity = new Qt3DCore::QEntity(m_rootEntity);
    coneEntity->addComponent(cone);
    coneEntity->addComponent(coneMaterial);
    coneEntity->addComponent(coneTransform);

    this->cone.append(coneEntity);
}

void SceneModifier::add_cylinder(const QVector3D &pos)
{
    auto *cylinder = new Qt3DExtras::QCylinderMesh();
    cylinder->setRadius(1);
    cylinder->setLength(3);
    cylinder->setRings(100);
    cylinder->setSlices(20);

    // CylinderMesh Transform
    auto *cylinderTransform = new Qt3DCore::QTransform();
    cylinderTransform->setScale(scaleFactor);
    //    cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));
    cylinderTransform->setTranslation(pos);

    auto *cylinderMaterial = new Qt3DExtras::QPhongMaterial();
    cylinderMaterial->setDiffuse(Qt::red);

    // Cylinder
    auto cylinderEntity = new Qt3DCore::QEntity(m_rootEntity);
    cylinderEntity->addComponent(cylinder);
    cylinderEntity->addComponent(cylinderMaterial);
    cylinderEntity->addComponent(cylinderTransform);

    this->cylinder.append(cylinderEntity);
}

void SceneModifier::add_cuboid(const QVector3D &pos)
{
    // Cuboid shape data
    auto *cuboid = new Qt3DExtras::QCuboidMesh();

    // CuboidMesh Transform
    auto *cuboidTransform = new Qt3DCore::QTransform();
    cuboidTransform->setScale(scaleFactor);
    cuboidTransform->setTranslation(pos);

    auto *cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    cuboidMaterial->setDiffuse(Qt::green);

    //Cuboid
    auto cuboidEntity = new Qt3DCore::QEntity(m_rootEntity);
    cuboidEntity->addComponent(cuboid);
    cuboidEntity->addComponent(cuboidMaterial);
    cuboidEntity->addComponent(cuboidTransform);

    this->cuboid.append(cuboidEntity);
}

void SceneModifier::add_plane(const QVector3D &pos)
{
    // Plane shape data
    auto *planeMesh = new Qt3DExtras::QPlaneMesh();
    planeMesh->setWidth(2);
    planeMesh->setHeight(2);

    // Plane mesh transform
    auto *planeTransform = new Qt3DCore::QTransform();
    planeTransform->setScale(1.3f);
    //    planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));
    planeTransform->setTranslation(pos);

    auto *planeMaterial = new Qt3DExtras::QPhongMaterial();
    planeMaterial->setDiffuse(QColor(QRgb(0xa69929)));

    // Plane
    auto planeEntity = new Qt3DCore::QEntity(m_rootEntity);
    planeEntity->addComponent(planeMesh);
    planeEntity->addComponent(planeMaterial);
    planeEntity->addComponent(planeTransform);

    this->plane.append(planeEntity);
}

void SceneModifier::add_sphere(const QVector3D &pos)
{
    // Sphere shape data
    auto *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(20);
    sphereMesh->setSlices(20);
    sphereMesh->setRadius(2);

    // Sphere mesh transform
    auto *sphereTransform = new Qt3DCore::QTransform();

    sphereTransform->setScale(scaleFactor);
    sphereTransform->setTranslation(pos);

    auto *sphereMaterial = new Qt3DExtras::QPhongMaterial();
    sphereMaterial->setDiffuse(Qt::yellow);

    // Sphere
    auto sphereEntity = new Qt3DCore::QEntity(m_rootEntity);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(sphereMaterial);
    sphereEntity->addComponent(sphereTransform);

    this->sphere.append(sphereEntity);
}

//! [4]
void SceneModifier::enableTorus(bool enabled)
{
    for (auto& shape : torus) {
        shape->setEnabled(enabled);
    }
}
//! [4]

void SceneModifier::enableCone(bool enabled)
{
    for (auto& shape : cone) {
        shape->setEnabled(enabled);
    }
}

void SceneModifier::enableCylinder(bool enabled)
{
    for (auto& shape : cylinder) {
        shape->setEnabled(enabled);
    }
}

void SceneModifier::enableCuboid(bool enabled)
{
    for (auto& shape : cuboid) {
        shape->setEnabled(enabled);
    }
}

void SceneModifier::enablePlane(bool enabled)
{
    for (auto& shape : plane) {
        shape->setEnabled(enabled);
    }
}

void SceneModifier::enableSphere(bool enabled)
{
    for (auto& shape : sphere) {
        shape->setEnabled(enabled);
    }
}
