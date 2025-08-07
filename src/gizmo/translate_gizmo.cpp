#include "gizmo/translate_gizmo.hpp"
#include "core/entity.hpp"
#include "scene/Camera.h"
#include "event/event_types.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Helper function for snapping
float snapValue(float value, float snap) {
    if (snap > 0.0f) {
        return std::round(value / snap) * snap;
    }
    return value;
}

TranslateGizmo::TranslateGizmo()
    : Gizmo(), m_target(nullptr) {
}

TranslateGizmo::TranslateGizmo(std::shared_ptr<Entity> target)
    : Gizmo(), m_target(target) {
}

TranslateGizmo::~TranslateGizmo() {}

void TranslateGizmo::draw(const Camera& camera) const {
    if (!m_target) return;

    // This will be implemented later with a proper rendering system
    // Use camera for view/projection matrices when rendering is implemented
    (void)camera;
}

bool TranslateGizmo::handleMousePressed(const event::MousePressEvent& e, const Camera& camera) {
    if (!m_target) return false;

    glm::vec3 rayOrigin, rayDir;
    screenToWorldRay(e.position.x, e.position.y, camera, rayOrigin, rayDir);

    m_hoveredAxis = hitTestAxis(rayOrigin, rayDir);
    if (m_hoveredAxis != Axis::None && e.isLeftButton) {
        m_isDragging = true;
        m_dragStartPosition = m_target->getPosition();
        
        // Project ray onto the drag axis to find the initial offset
        glm::vec3 axisDir = getAxisDirection(m_hoveredAxis);
        glm::vec3 toStart = m_dragStartPosition - rayOrigin;
        m_dragOffset = glm::dot(toStart, axisDir) / glm::dot(rayDir, axisDir);

        return true;
    }
    return false;
}

bool TranslateGizmo::handleMouseReleased(const event::MouseReleaseEvent& e, const Camera& camera) {
    if (m_isDragging && e.isLeftButton) {
        m_isDragging = false;
        m_hoveredAxis = Axis::None;
        // Camera could be used for coordinate system transforms during release
        (void)camera;
        return true;
    }
    return false;
}

bool TranslateGizmo::handleMouseMoved(const event::MouseMoveEvent& e, const Camera& camera) {
    if (!m_target) return false;

    glm::vec3 rayOrigin, rayDir;
    screenToWorldRay(e.position.x, e.position.y, camera, rayOrigin, rayDir);
    
    if (m_isDragging) {
        glm::vec3 axisDir = getAxisDirection(m_hoveredAxis);
        glm::vec3 newPos = rayOrigin + rayDir * m_dragOffset;
        
        // Project the new position onto the axis
        glm::vec3 offset = newPos - m_dragStartPosition;
        float projection = glm::dot(offset, axisDir);
        glm::vec3 projectedPos = m_dragStartPosition + axisDir * projection;

        if (m_snapEnabled) {
            float snappedProjection = snapValue(projection, m_snapDistance);
            projectedPos = m_dragStartPosition + axisDir * snappedProjection;
        }

        m_target->setPosition(projectedPos);
        return true;
    } else {
        // Update hover state
        Axis prevHovered = m_hoveredAxis;
        m_hoveredAxis = hitTestAxis(rayOrigin, rayDir);
        return m_hoveredAxis != prevHovered; // Return true if hover state changed
    }
}

bool TranslateGizmo::isHovered() const {
    return m_hoveredAxis != Axis::None;
}

Axis TranslateGizmo::hitTestAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const {
    if (!m_target) return Axis::None;

    float minDistance = std::numeric_limits<float>::max();
    Axis closestAxis = Axis::None;

    for (int i = 0; i < 3; ++i) {
        Axis axis = static_cast<Axis>(i);
        glm::vec3 axisDir = getAxisDirection(axis);
        float dist = distanceToLine(rayOrigin, rayDir, m_target->getPosition(), axisDir, m_size);
        
        if (dist < 0.1f) { // Hit threshold
            if (dist < minDistance) {
                minDistance = dist;
                closestAxis = axis;
            }
        }
    }
    return closestAxis;
}

float TranslateGizmo::distanceToLine(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& lineStart, const glm::vec3& lineDir, float lineLength) const {
    glm::vec3 crossP = glm::cross(rayDir, lineDir);
    float denominator = glm::length(crossP);

    if (denominator < 1e-6) { // Parallel lines
        return glm::length(glm::cross(lineStart - rayOrigin, rayDir)) / glm::length(rayDir);
    }

    glm::vec3 toLineStart = lineStart - rayOrigin;
    float t = glm::dot(glm::cross(toLineStart, lineDir), crossP) / (denominator * denominator);
    float s = glm::dot(glm::cross(toLineStart, rayDir), crossP) / (denominator * denominator);

    if (t >= 0 && s >= 0 && s <= lineLength) {
        return glm::length(rayOrigin + t * rayDir - (lineStart + s * lineDir));
    }

    return std::numeric_limits<float>::max();
}


glm::vec3 TranslateGizmo::getAxisDirection(Axis axis) const {
    switch (axis) {
        case Axis::X: return glm::vec3(1.0f, 0.0f, 0.0f);
        case Axis::Y: return glm::vec3(0.0f, 1.0f, 0.0f);
        case Axis::Z: return glm::vec3(0.0f, 0.0f, 1.0f);
        default: return glm::vec3(0.0f);
    }
}

glm::vec3 TranslateGizmo::getColor(Axis axis) const {
    switch (axis) {
        case Axis::X: return m_xColor;
        case Axis::Y: return m_yColor;
        case Axis::Z: return m_zColor;
        default: return glm::vec3(1.0f);
    }
}

glm::vec3 TranslateGizmo::getHoverColor(Axis axis) const {
    switch (axis) {
        case Axis::X: return m_xHoverColor;
        case Axis::Y: return m_yHoverColor;
        case Axis::Z: return m_zHoverColor;
        default: return glm::vec3(1.0f);
    }
}

void TranslateGizmo::screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const {
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