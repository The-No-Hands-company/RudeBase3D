//==============================================================================
// RudeBase3D Engine - scene.cpp
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


#include "core/scene.hpp"
#include "core/entity.hpp"
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <limits>
#include <utility>
#include <glm/glm.hpp>

rude::Scene::Scene() : nextEntityId_(0), entityNameCounter_(0) {
    // Scene is now initialized without PrimitiveManager - that's handled by SceneManager
}

rude::Entity* rude::Scene::createEntity(rude::PrimitiveType type, const std::string& name) {
    // Create entity with ID and type
    auto entity = std::make_unique<rude::Entity>(nextEntityId_++, type, name);
    
    // Note: Primitive mesh creation and assignment is now handled by SceneManager via PrimitiveManager
    // This follows better separation of concerns by removing direct primitive creation from Scene class
    rude::Entity* ptr = entity.get();
    entities_.push_back(std::move(entity));
    return ptr;
}

const std::vector<std::unique_ptr<rude::Entity>>& rude::Scene::getEntities() const {
    return entities_;
}

std::vector<rude::Entity*> rude::Scene::getRootEntities() const {
    std::vector<rude::Entity*> roots;
    for (const auto& entity : entities_) {
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

rude::Entity* rude::Scene::findEntityById(int id) const {
    for (const auto& entity : entities_) {
        if (entity && entity->getId() == id) return entity.get();
    }
    return nullptr;
}

void rude::Scene::removeEntity(rude::Entity* entity) {
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
    entities_.erase(std::remove_if(entities_.begin(), entities_.end(),
        [entity](const std::unique_ptr<Entity>& e) { return e.get() == entity; }), entities_.end());
}

void rude::Scene::clear() {
    entities_.clear();
    nextEntityId_ = 0;
}

glm::vec3 rude::Scene::getSceneBoundingBoxCenter() const {
    if (entities_.empty()) return glm::vec3(0.0f);
    glm::vec3 minPt( std::numeric_limits<float>::max() );
    glm::vec3 maxPt(-std::numeric_limits<float>::max() );
    bool found = false;
    for (const auto& entity : entities_) {
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

rude::Entity* rude::Scene::findEntityByName(const std::string& name) const {
    for (const auto& e : entities_) {
        if (e && e->getName() == name) return e.get();
    }
    return nullptr;
}

std::pair<glm::vec3, glm::vec3> rude::Scene::getSceneBoundingBox() const {
    if (entities_.empty()) return {glm::vec3(0.0f), glm::vec3(0.0f)};
    glm::vec3 minPt( std::numeric_limits<float>::max() );
    glm::vec3 maxPt(-std::numeric_limits<float>::max() );
    bool found = false;
    for (const auto& entity : entities_) {
        if (!entity) continue;
        auto [eMin, eMax] = entity->getAABB();
        minPt = glm::min(minPt, eMin);
        maxPt = glm::max(maxPt, eMax);
        found = true;
    }
    if (!found) return {glm::vec3(0.0f), glm::vec3(0.0f)};
    return {minPt, maxPt};
}

bool rude::Scene::isEmpty() const {
    return entities_.empty();
}

size_t rude::Scene::getEntityCount() const {
    return entities_.size();
}

std::string rude::Scene::generateEntityName(rude::PrimitiveType type) {
    std::string baseName;
    switch(type) {
        case rude::PrimitiveType::Cube: baseName = "Cube"; break;
        case rude::PrimitiveType::Sphere: baseName = "Sphere"; break;
        case rude::PrimitiveType::Cylinder: baseName = "Cylinder"; break;
        case rude::PrimitiveType::Plane: baseName = "Plane"; break;
        case rude::PrimitiveType::Icosphere: baseName = "Icosphere"; break;
        default: baseName = "Entity"; break;
    }
    return baseName + "_" + std::to_string(++entityNameCounter_);
}
