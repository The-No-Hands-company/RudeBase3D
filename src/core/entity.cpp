//==============================================================================
// RudeBase3D Engine - entity.cpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

#include "core/entity.hpp"
#include "core/qt_glm_utils.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

rude::Entity::Entity(int id, rude::PrimitiveType type, const std::string& name)
    : id(id), name(name), primitiveType(type) {}

int rude::Entity::getId() const {
    return id;
}

const std::string& rude::Entity::getName() const {
    return name;
}

Transform& rude::Entity::getTransform() {
    return transform;
}

rude::PrimitiveType rude::Entity::getPrimitiveType() const {
    return primitiveType;
}

// Return the mesh pointer (rude::MeshPtr)
rude::MeshPtr rude::Entity::getMesh() const {
    return mesh;
}

std::pair<glm::vec3, glm::vec3> rude::Entity::getAABB() const {
    return {aabb_min, aabb_max};
}

glm::mat4 rude::Entity::getWorldTransform() const {
    if (parent) {
        return parent->getWorldTransform() * transform.getModelMatrix();
    } else {
        return transform.getModelMatrix();
    }
}

void rude::Entity::draw(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) const {
    glm::mat4 world = parentTransform * transform.getModelMatrix();
    // TODO: Use 'world' for rendering this entity's mesh
    // Example: mesh->draw(world, view, proj);
    // Draw children recursively
    for (const auto& child : children) {
        if (child) child->draw(view, proj, world);
    }
}

void rude::Entity::addChild(Entity* child) {
    if (!child) return;
    // Remove from previous parent
    if (child->parent) {
        child->parent->removeChild(child);
    }
    children.push_back(child);
    child->parent = this;
}

void rude::Entity::removeChild(Entity* child) {
    if (!child) return;
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->parent = nullptr;
    }
}

void rude::Entity::setParent(Entity* newParent) {
    if (parent) {
        parent->removeChild(this);
    }
    if (newParent) {
        newParent->addChild(this);
    } else {
        parent = nullptr;
    }
}

glm::vec3 rude::Entity::getWorldPosition() const {
    // TODO: Implement proper world position calculation
    // For now, return the local position as a placeholder
    // In a complete implementation, this would:
    // 1. Get local transform/position
    // 2. Accumulate parent transforms up the hierarchy
    // 3. Return the final world space position
    return glm::vec3(0.0f, 0.0f, 0.0f);
}
