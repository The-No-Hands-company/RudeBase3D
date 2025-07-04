#include <algorithm>
#include "core/entity.hpp"
#include "core/qt_glm_utils.hpp"

Entity::Entity(int id, rude::PrimitiveType type, const std::string& name)
    : id(id), name(name), primitiveType(type) {}

int Entity::getId() const {
    return id;
}

const std::string& Entity::getName() const {
    return name;
}

Transform& Entity::getTransform() {
    return transform;
}

rude::PrimitiveType Entity::getPrimitiveType() const {
    return primitiveType;
}

std::shared_ptr<Mesh> Entity::getMesh() const {
    return mesh;
}

std::pair<glm::vec3, glm::vec3> Entity::getAABB() const {
    return {aabb_min, aabb_max};
}

glm::mat4 Entity::getWorldTransform() const {
    if (parent) {
        return parent->getWorldTransform() * rude::qMatrixToGlm(transform.getModelMatrix());
    } else {
        return rude::qMatrixToGlm(transform.getModelMatrix());
    }
}

void Entity::draw(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) const {
    glm::mat4 world = parentTransform * rude::qMatrixToGlm(transform.getModelMatrix());
    // TODO: Use 'world' for rendering this entity's mesh
    // Example: mesh->draw(world, view, proj);
    // Draw children recursively
    for (const auto& child : children) {
        if (child) child->draw(view, proj, world);
    }
}

void Entity::addChild(Entity* child) {
    if (!child) return;
    // Remove from previous parent
    if (child->parent) {
        child->parent->removeChild(child);
    }
    children.push_back(child);
    child->parent = this;
}

void Entity::removeChild(Entity* child) {
    if (!child) return;
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->parent = nullptr;
    }
}

void Entity::setParent(Entity* newParent) {
    if (parent) {
        parent->removeChild(this);
    }
    if (newParent) {
        newParent->addChild(this);
    } else {
        parent = nullptr;
    }
}
