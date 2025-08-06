#include "SceneObject.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshGenerator.h"

unsigned int SceneObject::s_nextId = 1;

SceneObject::SceneObject(const std::string& name)
    : m_id(s_nextId++)
    , m_name(name)
    , m_visible(true)
    , m_selected(false)
    , m_material(Material::createDefault())
{
}

void SceneObject::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, RenderMode mode)
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

glm::vec3 SceneObject::getBoundingBoxMin() const
{
    if (!m_mesh) {
        return m_transform.getPosition();
    }
    glm::vec3 meshMin = m_mesh->getBoundingBoxMin();
    glm::vec3 meshMax = m_mesh->getBoundingBoxMax();
    glm::mat4 modelMatrix = m_transform.getModelMatrix();
    glm::vec3 corners[8] = {
        glm::vec3(meshMin.x, meshMin.y, meshMin.z),
        glm::vec3(meshMin.x, meshMin.y, meshMax.z),
        glm::vec3(meshMin.x, meshMax.y, meshMin.z),
        glm::vec3(meshMin.x, meshMax.y, meshMax.z),
        glm::vec3(meshMax.x, meshMin.y, meshMin.z),
        glm::vec3(meshMax.x, meshMin.y, meshMax.z),
        glm::vec3(meshMax.x, meshMax.y, meshMin.z),
        glm::vec3(meshMax.x, meshMax.y, meshMax.z)
    };
    glm::vec3 worldMin = glm::vec3(modelMatrix * glm::vec4(corners[0], 1.0f));
    for (int i = 1; i < 8; ++i) {
        glm::vec3 worldCorner = glm::vec3(modelMatrix * glm::vec4(corners[i], 1.0f));
        worldMin.x = std::min(worldMin.x, worldCorner.x);
        worldMin.y = std::min(worldMin.y, worldCorner.y);
        worldMin.z = std::min(worldMin.z, worldCorner.z);
    }
    return worldMin;
}

glm::vec3 SceneObject::getBoundingBoxMax() const
{
    if (!m_mesh) {
        return m_transform.getPosition();
    }
    glm::vec3 meshMin = m_mesh->getBoundingBoxMin();
    glm::vec3 meshMax = m_mesh->getBoundingBoxMax();
    glm::mat4 modelMatrix = m_transform.getModelMatrix();
    glm::vec3 corners[8] = {
        glm::vec3(meshMin.x, meshMin.y, meshMin.z),
        glm::vec3(meshMin.x, meshMin.y, meshMax.z),
        glm::vec3(meshMin.x, meshMax.y, meshMin.z),
        glm::vec3(meshMin.x, meshMax.y, meshMax.z),
        glm::vec3(meshMax.x, meshMin.y, meshMin.z),
        glm::vec3(meshMax.x, meshMin.y, meshMax.z),
        glm::vec3(meshMax.x, meshMax.y, meshMin.z),
        glm::vec3(meshMax.x, meshMax.y, meshMax.z)
    };
    glm::vec3 worldMax = glm::vec3(modelMatrix * glm::vec4(corners[0], 1.0f));
    for (int i = 1; i < 8; ++i) {
        glm::vec3 worldCorner = glm::vec3(modelMatrix * glm::vec4(corners[i], 1.0f));
        worldMax.x = std::max(worldMax.x, worldCorner.x);
        worldMax.y = std::max(worldMax.y, worldCorner.y);
        worldMax.z = std::max(worldMax.z, worldCorner.z);
    }
    return worldMax;
}

glm::vec3 SceneObject::getBoundingBoxCenter() const
{
    return (getBoundingBoxMin() + getBoundingBoxMax()) * 0.5f;
}

SceneObjectPtr SceneObject::createCube(const std::string& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generateCube());
    return object;
}

SceneObjectPtr SceneObject::createSphere(const std::string& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generateSphere());
    return object;
}

SceneObjectPtr SceneObject::createCylinder(const std::string& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generateCylinder());
    return object;
}

SceneObjectPtr SceneObject::createPlane(const std::string& name)
{
    auto object = std::make_shared<SceneObject>(name);
    object->setMesh(MeshGenerator::generatePlane());
    return object;
}
