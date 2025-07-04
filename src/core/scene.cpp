#include "core/scene.hpp"
#include "core/entity.hpp"
#include "core/primitive_manager.hpp"
#include <algorithm>
#include <string>
#include <memory>
#include <glm/glm.hpp>

Scene::Scene() : nextId(0) {
    // Initialize primitive manager
    m_primitiveManager = std::make_unique<PrimitiveManager>();
}

Entity* Scene::createEntity(rude::PrimitiveType type, const std::string& name) {
    // Create entity with ID and type
    auto entity = std::make_unique<Entity>(nextId++, type, name);
    
    // Note: Primitive mesh creation and assignment is now handled by SceneManager via PrimitiveManager
    // This follows better separation of concerns by removing direct primitive creation from Scene class
    Entity* ptr = entity.get();
    entities.push_back(std::move(entity));
    return ptr;
}

const std::vector<std::unique_ptr<Entity>>& Scene::getEntities() const {
    return entities;
}

std::vector<Entity*> Scene::getRootEntities() const {
    std::vector<Entity*> roots;
    for (const auto& entity : entities) {
        if (entity && entity->getParent() == nullptr) {
            roots.push_back(entity.get());
        }
    }
    return roots;
}

void Scene::draw(const glm::mat4& view, const glm::mat4& proj) {
    for (Entity* root : getRootEntities()) {
        if (root) root->draw(view, proj);
    }
}

Entity* Scene::findEntityById(int id) const {
    for (const auto& entity : entities) {
        if (entity && entity->getId() == id) return entity.get();
    }
    return nullptr;
}

void Scene::removeEntity(Entity* entity) {
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

void Scene::clear() {
    entities.clear();
    nextId = 0;
}