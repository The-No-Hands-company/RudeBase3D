#include "SceneObject.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshGenerator.h"

unsigned int SceneObject::s_nextId = 1;

SceneObject::SceneObject(const QString& name)
    : m_id(s_nextId++)
    , m_name(name)
    , m_visible(true)
    , m_selected(false)
    , m_material(Material::createDefault())
{
}

void SceneObject::render(const QMatrix4x4& viewMatrix, const QMatrix4x4& projectionMatrix, RenderMode mode)
{
    Q_UNUSED(viewMatrix)
    Q_UNUSED(projectionMatrix)
    Q_UNUSED(mode)
    
    if (!m_visible || !m_mesh) {
        return;
    }
    
    // Rendering will be handled by the Renderer class
    // This method is called by the Scene to prepare object-specific data
}

QVector3D SceneObject::getBoundingBoxMin() const
{
    if (!m_mesh) {
        return m_transform.getPosition();
    }
    
    QVector3D meshMin = m_mesh->getBoundingBoxMin();
    QMatrix4x4 modelMatrix = m_transform.getModelMatrix();
    
    // Transform the bounding box corners
    QVector3D corners[8] = {
        QVector3D(meshMin.x(), meshMin.y(), meshMin.z()),
        QVector3D(meshMin.x(), meshMin.y(), m_mesh->getBoundingBoxMax().z()),
        QVector3D(meshMin.x(), m_mesh->getBoundingBoxMax().y(), meshMin.z()),
        QVector3D(meshMin.x(), m_mesh->getBoundingBoxMax().y(), m_mesh->getBoundingBoxMax().z()),
        QVector3D(m_mesh->getBoundingBoxMax().x(), meshMin.y(), meshMin.z()),
        QVector3D(m_mesh->getBoundingBoxMax().x(), meshMin.y(), m_mesh->getBoundingBoxMax().z()),
        QVector3D(m_mesh->getBoundingBoxMax().x(), m_mesh->getBoundingBoxMax().y(), meshMin.z()),
        QVector3D(m_mesh->getBoundingBoxMax().x(), m_mesh->getBoundingBoxMax().y(), m_mesh->getBoundingBoxMax().z())
    };
    
    QVector3D worldMin = modelMatrix.map(corners[0]);
    for (int i = 1; i < 8; ++i) {
        QVector3D worldCorner = modelMatrix.map(corners[i]);
        worldMin.setX(std::min(worldMin.x(), worldCorner.x()));
        worldMin.setY(std::min(worldMin.y(), worldCorner.y()));
        worldMin.setZ(std::min(worldMin.z(), worldCorner.z()));
    }
    
    return worldMin;
}

QVector3D SceneObject::getBoundingBoxMax() const
{
    if (!m_mesh) {
        return m_transform.getPosition();
    }
    
    QVector3D meshMin = m_mesh->getBoundingBoxMin();
    QVector3D meshMax = m_mesh->getBoundingBoxMax();
    QMatrix4x4 modelMatrix = m_transform.getModelMatrix();
    
    // Transform the bounding box corners
    QVector3D corners[8] = {
        QVector3D(meshMin.x(), meshMin.y(), meshMin.z()),
        QVector3D(meshMin.x(), meshMin.y(), meshMax.z()),
        QVector3D(meshMin.x(), meshMax.y(), meshMin.z()),
        QVector3D(meshMin.x(), meshMax.y(), meshMax.z()),
        QVector3D(meshMax.x(), meshMin.y(), meshMin.z()),
        QVector3D(meshMax.x(), meshMin.y(), meshMax.z()),
        QVector3D(meshMax.x(), meshMax.y(), meshMin.z()),
        QVector3D(meshMax.x(), meshMax.y(), meshMax.z())
    };
    
    QVector3D worldMax = modelMatrix.map(corners[0]);
    for (int i = 1; i < 8; ++i) {
        QVector3D worldCorner = modelMatrix.map(corners[i]);
        worldMax.setX(std::max(worldMax.x(), worldCorner.x()));
        worldMax.setY(std::max(worldMax.y(), worldCorner.y()));
        worldMax.setZ(std::max(worldMax.z(), worldCorner.z()));
    }
    
    return worldMax;
}

QVector3D SceneObject::getBoundingBoxCenter() const
{
    return (getBoundingBoxMin() + getBoundingBoxMax()) * 0.5f;
}

SceneObjectPtr SceneObject::createCube(const QString& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generateCube());
    return object;
}

SceneObjectPtr SceneObject::createSphere(const QString& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generateSphere());
    return object;
}

SceneObjectPtr SceneObject::createCylinder(const QString& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generateCylinder());
    return object;
}

SceneObjectPtr SceneObject::createPlane(const QString& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generatePlane());
    return object;
}
