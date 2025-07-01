#include "SceneManager.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshGenerator.h"
#include <QDebug>
#include <QFileInfo>

SceneManager::SceneManager(QObject* parent)
    : QObject(parent)
    , m_nextObjectId(1)
{
    // Don't auto-create scene in constructor
    // Let MainWindow explicitly call newScene() when ready
}

void SceneManager::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
    
    if (m_scene) {
        // Connect scene signals to manager signals
        connect(m_scene.get(), &Scene::objectAdded, this, &SceneManager::objectAdded);
        connect(m_scene.get(), &Scene::objectRemoved, this, &SceneManager::objectRemoved);
        connect(m_scene.get(), &Scene::selectionChanged, this, &SceneManager::selectionChanged);
        connect(m_scene.get(), &Scene::sceneChanged, this, &SceneManager::sceneChanged);
    }
}

void SceneManager::newScene()
{
    m_scene = std::make_shared<Scene>(this);
    m_nextObjectId = 1;
    
    setScene(m_scene);
    setupDefaultScene();
    
    emit sceneChanged();
}

void SceneManager::clearScene()
{
    if (m_scene) {
        m_scene->clear();
        m_nextObjectId = 1;
        emit sceneChanged();
    }
}

bool SceneManager::loadScene(const QString& filePath)
{
    // TODO: Implement scene loading from file
    // For now, just create a test scene
    Q_UNUSED(filePath)
    
    qDebug() << "Scene loading not yet implemented";
    return false;
}

bool SceneManager::saveScene(const QString& filePath)
{
    // TODO: Implement scene saving to file
    Q_UNUSED(filePath)
    
    qDebug() << "Scene saving not yet implemented";
    return false;
}

SceneObjectPtr SceneManager::createCube()
{
    auto mesh = MeshGenerator::generateCube(1.0f);
    return createPrimitiveObject("Cube", mesh);
}

SceneObjectPtr SceneManager::createSphere()
{
    auto mesh = MeshGenerator::generateSphere(0.5f, 32, 16);
    return createPrimitiveObject("Sphere", mesh);
}

SceneObjectPtr SceneManager::createPlane()
{
    auto mesh = MeshGenerator::generatePlane(2.0f, 2.0f);
    return createPrimitiveObject("Plane", mesh);
}

SceneObjectPtr SceneManager::createCylinder()
{
    auto mesh = MeshGenerator::generateCylinder(0.5f, 1.0f, 32);
    return createPrimitiveObject("Cylinder", mesh);
}

SceneObjectPtr SceneManager::createCone()
{
    auto mesh = MeshGenerator::generateCone(0.5f, 1.0f, 32);
    return createPrimitiveObject("Cone", mesh);
}

SceneObjectPtr SceneManager::createTorus()
{
    auto mesh = MeshGenerator::generateTorus(0.5f, 0.2f, 32, 16);
    return createPrimitiveObject("Torus", mesh);
}

SceneObjectPtr SceneManager::createIcosphere()
{
    auto mesh = MeshGenerator::generateIcosphere(0.5f, 2);
    return createPrimitiveObject("Icosphere", mesh);
}

void SceneManager::addObject(SceneObjectPtr object)
{
    if (m_scene && object) {
        m_scene->addObject(object);
    }
}

void SceneManager::removeObject(SceneObjectPtr object)
{
    if (m_scene && object) {
        m_scene->removeObject(object);
    }
}

void SceneManager::duplicateObject(SceneObjectPtr object)
{
    if (!object || !m_scene) return;
    
    // Create a duplicate object
    auto duplicate = std::make_shared<SceneObject>(object->getName() + "_Copy");
    duplicate->setMesh(object->getMesh());
    duplicate->setMaterial(object->getMaterial());
    
    // Offset the position slightly
    Transform transform = object->getTransform();
    QVector3D position = transform.getPosition();
    position += QVector3D(1.0f, 0.0f, 0.0f); // Offset by 1 unit in X
    transform.setPosition(position);
    duplicate->getTransform() = transform;
    
    m_scene->addObject(duplicate);
}

void SceneManager::selectObject(SceneObjectPtr object)
{
    if (m_scene) {
        m_scene->setSelectedObject(object);
    }
}

void SceneManager::clearSelection()
{
    if (m_scene) {
        m_scene->clearSelection();
    }
}

SceneObjectPtr SceneManager::getSelectedObject() const
{
    if (m_scene) {
        return m_scene->getSelectedObject();
    }
    return nullptr;
}

size_t SceneManager::getObjectCount() const
{
    if (m_scene) {
        return m_scene->getObjectCount();
    }
    return 0;
}

bool SceneManager::isEmpty() const
{
    if (m_scene) {
        return m_scene->isEmpty();
    }
    return true;
}

void SceneManager::setupDefaultScene()
{
    if (!m_scene) return;
    
    // Modern professional 3D applications start with an empty scene
    // No default test objects - users create geometry as needed
    
    // TODO: Add optional reference grid or ground plane
    // TODO: Set up professional lighting rig (3-point lighting, etc.)
    
    qDebug() << "Professional empty scene created - ready for modeling";
}

SceneObjectPtr SceneManager::createPrimitiveObject(const QString& name, std::shared_ptr<Mesh> mesh)
{
    if (!mesh) return nullptr;
    
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(mesh);
    
    // Use the standard industry default material
    auto material = Material::createDefault();
    object->setMaterial(material);
    
    if (m_scene) {
        m_scene->addObject(object);
    }
    
    return object;
}
