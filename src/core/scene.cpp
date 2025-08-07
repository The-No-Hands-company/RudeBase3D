
#include "core/scene.hpp"
#include "core/entity.hpp"
#include "core/primitive_manager.hpp"
#include <algorithm>
#include <string>
#include <memory>
#include <glm/glm.hpp>

rude::Scene::Scene() : nextId(0) {
    // Initialize primitive manager
    m_primitiveManager = std::make_unique<PrimitiveManager>();
}

Entity* rude::Scene::createEntity(rude::PrimitiveType type, const std::string& name) {
    // Create entity with ID and type
    auto entity = std::make_unique<Entity>(nextId++, type, name);
    
    // Note: Primitive mesh creation and assignment is now handled by SceneManager via PrimitiveManager
    // This follows better separation of concerns by removing direct primitive creation from Scene class
    Entity* ptr = entity.get();
    entities.push_back(std::move(entity));
    return ptr;
}

const std::vector<std::unique_ptr<Entity>>& rude::Scene::getEntities() const {
    return entities;
}

std::vector<Entity*> rude::Scene::getRootEntities() const {
    std::vector<Entity*> roots;
    for (const auto& entity : entities) {
        if (entity && entity->getParent() == nullptr) {
            roots.push_back(entity.get());
        }
    }
    return roots;
}

void rude::Scene::draw(const glm::mat4& view, const glm::mat4& proj) {
    for (Entity* root : getRootEntities()) {
        if (root) root->draw(view, proj);
    }
}

Entity* rude::Scene::findEntityById(int id) const {
    for (const auto& entity : entities) {
        if (entity && entity->getId() == id) return entity.get();
    }
    return nullptr;
}

void rude::Scene::removeEntity(Entity* entity) {
    if (!entity) return;
    // Remove from parent if needed
    if (entity->getParent()) {
        entity->getParent()->removeChild(entity);
    }
    // Recursively remove children
    for (Entity* child : entity->getChildren()) {
        removeEntity(child);
    }
    // Remove from entities vector
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [entity](const std::unique_ptr<Entity>& e) { return e.get() == entity; }), entities.end());
}

void rude::Scene::clear() {
    entities.clear();
    nextId = 0;
}

glm::vec3 rude::Scene::getSceneBoundingBoxCenter() const {
    if (entities.empty()) return glm::vec3(0.0f);
    glm::vec3 minPt( std::numeric_limits<float>::max() );
    glm::vec3 maxPt(-std::numeric_limits<float>::max() );
    bool found = false;
    for (const auto& entity : entities) {
        if (!entity) continue;
        // Use Entity::getAABB() which returns (min, max)
        auto [eMin, eMax] = entity->getAABB();
        minPt = glm::min(minPt, eMin);
        maxPt = glm::max(maxPt, eMax);
        found = true;
    }
    if (!found) return glm::vec3(0.0f);
    return (minPt + maxPt) * 0.5f;
}


namespace rude {

void Scene::deleteEntity(Entity* entity) {
    removeEntity(entity);
}

Entity* Scene::duplicateEntity(Entity* entity) {
    if (!entity) return nullptr;
    // Simple shallow copy: create new entity with same type and name + _copy
    std::string newName = entity->getName();
    newName += "_copy";
    Entity* copy = createEntity(entity->getPrimitiveType(), newName);
    // TODO: Deep copy components, children, etc.
    return copy;
}

std::vector<Entity*> Scene::getAllEntities() const {
    std::vector<Entity*> result;
    for (const auto& e : entities) {
        if (e) result.push_back(e.get());
    }
    return result;
}

Entity* Scene::findEntityByName(const QString& name) const {
    for (const auto& e : entities) {
        if (e && QString::fromStdString(e->getName()) == name) return e.get();
    }
    return nullptr;
}

Entity* Scene::findEntityByName(const std::string& name) const {
    for (const auto& e : entities) {
        if (e && e->getName() == name) return e.get();
    }
    return nullptr;
}

Entity* Scene::findEntityById(uint32_t id) const {
    for (const auto& entity : entities) {
        if (entity && static_cast<uint32_t>(entity->getId()) == id) return entity.get();
    }
    return nullptr;
}

bool Scene::isEmpty() const {
    return entities.empty();
}

glm::vec3 Scene::getBoundingBoxCenter() const {
    return getSceneBoundingBoxCenter();
}

} // namespace rude
