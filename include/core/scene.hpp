#pragma once
#include <vector>
#include <memory>
#include "entity.hpp"
#include <glm/glm.hpp>
#include "primitive_manager.hpp"  // Include full definition instead of forward declaration

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

private:
    int nextId;
    std::vector<std::unique_ptr<Entity>> entities;
    std::unique_ptr<PrimitiveManager> m_primitiveManager;
};