#include "gizmo/scale_gizmo.hpp"
#include "core/entity.hpp"
#include "scene/Camera.h"
#include "event/event_types.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Helper function for snapping
float snapScale(float value, float snap) {
    if (snap > 0.0f) {
        return std::round(value / snap) * snap;
    }
    return value;
}

ScaleGizmo::ScaleGizmo()
    : Gizmo(), m_target(nullptr) {
}

ScaleGizmo::ScaleGizmo(std::shared_ptr<rude::Entity> target)
    : Gizmo(), m_target(target) {
}

ScaleGizmo::~ScaleGizmo() {}

void ScaleGizmo::draw(const Camera& camera) const {
    if (!m_target) return;
    // Drawing logic will be implemented later
    // Camera will be used for view/projection matrices during rendering
    (void)camera;
}

bool ScaleGizmo::handleMousePressed(const event::MousePressEvent& e, const Camera& camera) {
    if (!m_target) return false;

    glm::vec3 rayOrigin, rayDir;
    screenToWorldRay(e.position.x, e.position.y, camera, rayOrigin, rayDir);

    m_hoveredAxis = hitTestAxis(rayOrigin, rayDir);
    if (m_hoveredAxis != Axis::None && e.isLeftButton) {
        m_isDragging = true;
        m_activeAxis = m_hoveredAxis;
        m_startScale = m_target->getScale();
        m_startDragPosition = m_target->getPosition();
        return true;
    }
    return false;
}

bool ScaleGizmo::handleMouseReleased(const event::MouseReleaseEvent& e, const Camera& camera) {
    if (m_isDragging && e.isLeftButton) {
        m_isDragging = false;
        m_activeAxis = Axis::None;
        // Camera could be used for coordinate transforms during release
        (void)camera;
        return true;
    }
    return false;
}

bool ScaleGizmo::handleMouseMoved(const event::MouseMoveEvent& e, const Camera& camera) {
    if (!m_target) return false;

    glm::vec3 rayOrigin, rayDir;
    screenToWorldRay(e.position.x, e.position.y, camera, rayOrigin, rayDir);
    
    if (m_isDragging) {
        glm::vec3 axisDir = getAxisDirection(m_activeAxis);
        
        // Project mouse movement onto the axis
        glm::vec3 planeNormal = camera.getForward();
        glm::vec3 linePoint = m_target->getPosition();
        
        float nd = glm::dot(rayDir, planeNormal);
        // Check if ray is parallel to plane
        if (std::abs(nd) < 1e-6) return false;

        float t = (glm::dot(linePoint, planeNormal) - glm::dot(rayOrigin, planeNormal)) / nd;
        glm::vec3 mouseOnPlane = rayOrigin + t * rayDir;

        float dragAmount = glm::dot(mouseOnPlane - m_startDragPosition, axisDir);

        if (m_snapEnabled) {
            dragAmount = snapScale(dragAmount, m_snapScale);
        }

        // Update scale (make sure it doesn't go negative)
        glm::vec3 newScale = m_startScale;
        int axisIndex = static_cast<int>(m_activeAxis);
        newScale[axisIndex] = std::max(m_startScale[axisIndex] + dragAmount, 0.01f);

        m_target->setScale(newScale);
        return true;
    } else {
        // Update hover state
        Axis prevHovered = m_hoveredAxis;
        m_hoveredAxis = hitTestAxis(rayOrigin, rayDir);
        return m_hoveredAxis != prevHovered; // Return true if hover state changed
    }
}

bool ScaleGizmo::isHovered() const {
    return m_hoveredAxis != Axis::None;
}

Axis ScaleGizmo::hitTestAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const {
    if (!m_target) return Axis::None;

    // rayDir would be used for proper ray-object intersection testing
    // For now, acknowledge parameter for future ray-casting implementation
    (void)rayDir;

    float minDistance = ::std::numeric_limits<float>::max();
    Axis closestAxis = Axis::None;

    for (int i = 0; i < 3; ++i) {
        Axis axis = static_cast<Axis>(i);
        glm::vec3 axisDir = getAxisDirection(axis);
        glm::vec3 cubeCenter = m_target->getPosition() + axisDir * m_size;
        
        // A simple bounding box check for the cube at the end of the axis
        float dist = distanceToCube(rayOrigin, cubeCenter, axisDir, 0.1f); // 0.1f is cube size
        if (dist < ::std::numeric_limits<float>::max()) {
             if (dist < minDistance) {
                minDistance = dist;
                closestAxis = axis;
            }
        }
    }
    return closestAxis;
}

float ScaleGizmo::distanceToCube(const glm::vec3& point, const glm::vec3& center, const glm::vec3& direction, float size) const
{
    // This is a simplified placeholder. A real implementation would be more robust.
    // Direction would be used for oriented bounding box calculations
    (void)direction;
    
    glm::vec3 min = center - glm::vec3(size);
    glm::vec3 max = center + glm::vec3(size);
    if (point.x > min.x && point.x < max.x &&
        point.y > min.y && point.y < max.y &&
        point.z > min.z && point.z < max.z) {
        return glm::distance(point, center);
    }
    return ::std::numeric_limits<float>::max();
}

void ScaleGizmo::drawCube(const glm::vec3& position, const glm::vec3& direction, float size, const glm::vec3& color) const {
    // TODO: Implement cube drawing
    // For now, this is a stub implementation that acknowledges all parameters
    (void)position;  // Future: world position for cube placement
    (void)direction; // Future: orientation for cube alignment
    (void)size;      // Future: scale factor for cube dimensions
    (void)color;     // Future: color for cube rendering
}



glm::vec3 getAxisDirection(Axis axis) {
    switch (axis) {
        case Axis::X: return glm::vec3(1.0f, 0.0f, 0.0f);
        case Axis::Y: return glm::vec3(0.0f, 1.0f, 0.0f);
        case Axis::Z: return glm::vec3(0.0f, 0.0f, 1.0f);
        default: return glm::vec3(0.0f);
    }
}

void ScaleGizmo::screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const {
    glm::mat4 view = camera.getViewMatrixGLM();
    glm::mat4 proj = camera.getProjectionMatrixGLM();
    float viewportWidth = camera.getViewportWidth();
    float viewportHeight = camera.getViewportHeight();

    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, viewportWidth, viewportHeight);
    glm::vec3 screenPos(x, viewportHeight - y, 0.0f); // Invert Y

    glm::vec3 nearPoint = glm::unProject(glm::vec3(screenPos.x, screenPos.y, 0.0f), view, proj, viewport);
    glm::vec3 farPoint = glm::unProject(glm::vec3(screenPos.x, screenPos.y, 1.0f), view, proj, viewport);

    outOrigin = nearPoint;
    outDir = glm::normalize(farPoint - nearPoint);
}
