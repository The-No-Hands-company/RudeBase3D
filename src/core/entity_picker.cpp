//==============================================================================
// RudeBase3D Engine - entity_picker.cpp
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

#define GLM_ENABLE_EXPERIMENTAL
#include "core/entity_picker.hpp"
#include "core/scene.hpp"
#include "core/entity.hpp"
#include "scene/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <limits>

EntityPicker::EntityPicker() {}
void EntityPicker::setScene(rude::Scene* s) { scene = s; }
void EntityPicker::setCamera(Camera* c) { camera = c; }
rude::Entity* EntityPicker::getSelectedEntity() const { return selectedEntity; }

void EntityPicker::pickEntity(float mouseX, float mouseY, int viewportWidth, int viewportHeight) {
    if (!scene || !camera) return;
    float x = (2.0f * mouseX) / viewportWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / viewportHeight;
    glm::vec4 ray_clip(x, y, -1.0, 1.0);
    glm::mat4 invProj = glm::inverse(camera->getProjectionMatrixGLM());
    glm::vec4 ray_eye = invProj * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    glm::mat4 invView = glm::inverse(camera->getViewMatrixGLM());
    glm::vec4 ray_wor = invView * ray_eye;
    glm::vec3 ray_dir = glm::normalize(glm::vec3(ray_wor));
    glm::vec3 ray_origin = camera->getPosition();
    float closest = 1e9f;
    selectedEntity = nullptr;
    for (const auto& entityPtr : scene->getEntities()) {
        auto aabb = entityPtr->getAABB();
        float tmin = 0, tmax = 0;
        if (rayAABBIntersect(ray_origin, ray_dir, aabb.first, aabb.second, tmin, tmax)) {
            if (tmin < closest) {
                closest = tmin;
                selectedEntity = entityPtr.get();
            }
        }
    }
}

bool EntityPicker::rayAABBIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& tmin, float& tmax) {
    tmin = (aabbMin.x - rayOrigin.x) / rayDir.x;
    tmax = (aabbMax.x - rayOrigin.x) / rayDir.x;
    if (tmin > tmax) std::swap(tmin, tmax);
    float tymin = (aabbMin.y - rayOrigin.y) / rayDir.y;
    float tymax = (aabbMax.y - rayOrigin.y) / rayDir.y;
    if (tymin > tymax) std::swap(tymin, tymax);
    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    float tzmin = (aabbMin.z - rayOrigin.z) / rayDir.z;
    float tzmax = (aabbMax.z - rayOrigin.z) / rayDir.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);
    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    return tmax > 0;
}
