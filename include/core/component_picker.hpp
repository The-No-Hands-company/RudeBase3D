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
