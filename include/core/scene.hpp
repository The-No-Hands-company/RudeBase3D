#pragma once
#include <vector>
#include <memory>
#include "entity.hpp"
#include <glm/glm.hpp>
#include "primitive_manager.hpp"  // Include full definition instead of forward declaration

namespace rude {

class Scene {
public:
    Scene();
    Entity* createEntity(rude::PrimitiveType type, const std::string& name = "");
    const std::vector<std::unique_ptr<Entity>>& getEntities() const;
    // Get root entities (no parent)
    std::vector<Entity*> getRootEntities() const;
    void draw(const glm::mat4& view, const glm::mat4& proj);

    // Entity removal and scene management
    void removeEntity(Entity* entity);
    void clear();
    Entity* findEntityById(int id) const;
    Entity* findEntityById(uint32_t id) const;

    // Modern API additions
    void deleteEntity(Entity* entity);
    Entity* duplicateEntity(Entity* entity);
    std::vector<Entity*> getAllEntities() const;
    Entity* findEntityByName(const std::string& name) const;
    Entity* findEntityByName(const QString& name) const;
    bool isEmpty() const;
    glm::vec3 getBoundingBoxCenter() const;

    // Bounding box center for all entities
    glm::vec3 getSceneBoundingBoxCenter() const;

private:
    int nextId;
    std::vector<std::unique_ptr<Entity>> entities;
    std::unique_ptr<PrimitiveManager> m_primitiveManager;
};

} // namespace rude