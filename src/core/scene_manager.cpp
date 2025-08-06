
#include "core/scene_manager.hpp"
#include "core/primitive_manager.hpp"
#include <glm/glm.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

namespace rude {

SceneManager::SceneManager(QObject* parent)
    : QObject(parent), m_scene(std::make_shared<rude::Scene>()), m_primitiveManager(nullptr), m_externalPrimitiveManager(nullptr), m_selectedEntity(nullptr)
{
}

SceneManager::~SceneManager() = default;

void SceneManager::setScene(std::shared_ptr<rude::Scene> scene) {
    m_scene = scene;
    emit sceneLoaded("");
}

void SceneManager::setPrimitiveManager(PrimitiveManager* primitiveManager) {
    m_externalPrimitiveManager = primitiveManager;
}

Entity* SceneManager::createPrimitive(const std::string& primitiveType, const std::string& name) {
    // Map primitiveType string to rude::PrimitiveType enum if possible
    rude::PrimitiveType type = rude::PrimitiveType::Unknown;
    std::string typeStr = primitiveType;
    if (typeStr == "cube") type = rude::PrimitiveType::Cube;
    else if (typeStr == "sphere") type = rude::PrimitiveType::Sphere;
    else if (typeStr == "cylinder") type = rude::PrimitiveType::Cylinder;
    else if (typeStr == "plane") type = rude::PrimitiveType::Plane;
    else if (typeStr == "cone") type = rude::PrimitiveType::Cone;
    else if (typeStr == "torus") type = rude::PrimitiveType::Torus;
    else if (typeStr == "icosphere") type = rude::PrimitiveType::Icosphere;
    // Add more as needed
    std::string entityName = name.empty() ? typeStr : name;
    Entity* entity = nullptr;
    if (type != rude::PrimitiveType::Unknown) {
        entity = m_scene ? m_scene->createEntity(type, entityName) : nullptr;
    } else {
        entity = m_scene ? m_scene->createEntity(rude::PrimitiveType::Unknown, entityName) : nullptr;
    }
    emit entityCreated(entity);
    return entity;
}

Entity* SceneManager::createEntity(const std::string& name) {
    if (!m_scene) return nullptr;
    std::string entityName = name;
    // Default to Unknown type
    auto entity = m_scene->createEntity(rude::PrimitiveType::Unknown, entityName);
    connectEntitySignals(entity);
    emit entityCreated(entity);
    return entity;
}

Entity* SceneManager::importMesh(const std::string& filePath, const std::string& name) {
    // Stub: create entity and mark as imported
    Entity* entity = createEntity(name.empty() ? filePath : name);
    // TODO: Load mesh from file and assign to entity
    emit entityCreated(entity);
    return entity;
}

void SceneManager::deleteEntity(Entity* entity) {
    if (!m_scene || !entity) return;
    m_scene->deleteEntity(entity);
    emit entityDeleted(entity);
}

void SceneManager::deleteSelectedEntities() {
    if (!m_scene || !m_selectedEntity) return;
    deleteEntity(m_selectedEntity);
    m_selectedEntity = nullptr;
    emit selectionChanged();
}

void SceneManager::duplicateEntity(Entity* entity) {
    if (!m_scene || !entity) return;
    Entity* copy = m_scene->duplicateEntity(entity);
    connectEntitySignals(copy);
    emit entityCreated(copy);
}

void SceneManager::clearScene() {
    if (m_scene) m_scene->clear();
    m_selectedEntity = nullptr;
    emit sceneCleared();
}

bool SceneManager::saveScene(const std::string& filePath) {
    // Stub: serialize scene to file
    if (!m_scene) return false;
    std::ofstream file(filePath);
    if (!file.is_open()) return false;
    file << "Scene serialization not implemented";
    file.close();
    emit sceneSaved(filePath);
    return true;
}

bool SceneManager::loadScene(const std::string& filePath) {
    // Stub: load scene from file
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    // TODO: Deserialize scene
    file.close();
    emit sceneLoaded(filePath);
    return true;
}

std::vector<Entity*> SceneManager::getAllEntities() const {
    if (!m_scene) return {};
    return m_scene->getAllEntities();
}

Entity* SceneManager::findEntityByName(const std::string& name) const {
    if (!m_scene) return nullptr;
    return m_scene->findEntityByName(name);
}

Entity* SceneManager::findEntityById(uint32_t id) const {
    if (!m_scene) return nullptr;
    return m_scene->findEntityById(id);
}

Entity* SceneManager::pickObject(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
    // Stub: implement picking logic
    if (!m_scene) return nullptr;
    // Convert to glm::vec3 if needed
    return nullptr;
}

void SceneManager::setSelectedObject(Entity* entity) {
    m_selectedEntity = entity;
    emit selectionChanged();
}

bool SceneManager::isEmpty() const {
    return !m_scene || m_scene->isEmpty();
}

glm::vec3 SceneManager::getSceneBoundingBoxCenter() const {
    if (!m_scene) return glm::vec3();
    return m_scene->getBoundingBoxCenter();
}

std::string SceneManager::generateEntityName(const std::string& baseName) const {
    // Stub: generate a unique name
    return baseName + "_1";
}

void SceneManager::connectEntitySignals(Entity* entity) {
    // Stub: connect signals if needed
    Q_UNUSED(entity);
}

} // namespace rude
