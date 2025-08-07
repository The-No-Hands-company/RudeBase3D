//==============================================================================
// RudeBase3D Engine - component_picker.hpp
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

#pragma once

#include "core/selection_manager.hpp"
#include "scene/Camera.h"
#include <glm/glm.hpp>
#include <vector>

class Entity; // Forward declaration for Entity in global scope

namespace rude {

class ComponentPicker {
public:
    ComponentPicker();
    ~ComponentPicker() = default;
    
    // Configuration
    void setCamera(Camera* camera) { m_camera = camera; }
    void setSelectionManager(SelectionManager* manager) { m_selectionManager = manager; }
    void setPickingTolerance(float tolerance) { m_pickingTolerance = tolerance; }
    
    // Picking operations
    SelectionData pickComponent(float mouseX, float mouseY, int viewportWidth, int viewportHeight, 
                               ComponentType targetType, ::Entity* targetEntity = nullptr);
    
    ::std::vector<SelectionData> pickComponentsInRegion(float x1, float y1, float x2, float y2,
                                                     int viewportWidth, int viewportHeight,
                                                     ComponentType targetType, ::Entity* targetEntity = nullptr);
    
    // Ray-casting utilities
    glm::vec3 screenToWorldRay(float mouseX, float mouseY, int viewportWidth, int viewportHeight) const;
    glm::vec3 getRayOrigin(float mouseX, float mouseY, int viewportWidth, int viewportHeight) const;
    glm::vec3 getRayDirection(float mouseX, float mouseY, int viewportWidth, int viewportHeight) const;
    
private:
    Camera* m_camera = nullptr;
    SelectionManager* m_selectionManager = nullptr;
    float m_pickingTolerance = 5.0f; // pixels
    
    // Component picking methods
    SelectionData pickVertex(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, ::Entity* entity);
    SelectionData pickEdge(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, ::Entity* entity);
    SelectionData pickFace(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, ::Entity* entity);
    
    // Intersection tests
    float pointToRayDistance(const glm::vec3& point, const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;
    float lineSegmentToRayDistance(const glm::vec3& p1, const glm::vec3& p2, 
                                  const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;
    bool rayTriangleIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                             const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                             float& t, glm::vec3& intersection) const;
    
    // Screen space utilities
    glm::vec3 worldToScreen(const glm::vec3& worldPos, int viewportWidth, int viewportHeight) const;
    float screenSpaceDistance(const glm::vec3& worldPos, float mouseX, float mouseY,
                             int viewportWidth, int viewportHeight) const;
};

} // namespace rude
