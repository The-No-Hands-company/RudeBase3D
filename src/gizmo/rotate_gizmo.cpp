#define GLM_ENABLE_EXPERIMENTAL
#include "gizmo/rotate_gizmo.hpp"
#include "core/entity.hpp"
#include "scene/Camera.h"
#include "event/event_types.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

// Helper function for snapping
float snapAngle(float angle, float snap) {
    if (snap > 0.0f) {
        return std::round(angle / snap) * snap;
    }
    return angle;
}

RotateGizmo::RotateGizmo()
    : Gizmo(), m_target(nullptr) {
}

RotateGizmo::RotateGizmo(std::shared_ptr<Entity> target)
    : Gizmo(), m_target(target) {
}

RotateGizmo::~RotateGizmo() {}

void RotateGizmo::draw(const Camera& camera) const {
    if (!m_target) return;
    // Drawing logic will be implemented later
}

bool RotateGizmo::handleMousePressed(const event::MousePressEvent& e, const Camera& camera) {
    if (!m_target) return false;

    glm::vec3 rayOrigin, rayDir;
    screenToWorldRay(e.position.x, e.position.y, camera, rayOrigin, rayDir);

    m_hoveredAxis = hitTestAxis(rayOrigin, rayDir);
    if (m_hoveredAxis != Axis::None && e.isLeftButton) {
        m_isDragging = true;
        m_activeAxis = m_hoveredAxis;
        m_startRotation = glm::eulerAngles(m_target->getRotation());

        // Calculate the initial intersection point on the rotation plane
        glm::vec3 axisDir = getAxisDirection(m_activeAxis);
        glm::vec3 planePoint = m_target->getPosition();
        float d = glm::dot(planePoint - rayOrigin, axisDir) / glm::dot(rayDir, axisDir);
        m_startDragWorld = rayOrigin + d * rayDir;
        
        return true;
    }
    return false;
}

bool RotateGizmo::handleMouseReleased(const event::MouseReleaseEvent& e, const Camera& camera) {
    if (m_isDragging && e.isLeftButton) {
        m_isDragging = false;
        m_activeAxis = Axis::None;
        return true;
    }
    return false;
}

bool RotateGizmo::handleMouseMoved(const event::MouseMoveEvent& e, const Camera& camera) {
    if (!m_target) return false;

    glm::vec3 rayOrigin, rayDir;
    screenToWorldRay(e.position.x, e.position.y, camera, rayOrigin, rayDir);
    
    if (m_isDragging) {
        // Intersect with the plane again
        glm::vec3 axisDir = getAxisDirection(m_activeAxis);
        glm::vec3 planePoint = m_target->getPosition();
        float d = glm::dot(planePoint - rayOrigin, axisDir) / glm::dot(rayDir, axisDir);
        glm::vec3 currentDragWorld = rayOrigin + d * rayDir;

        glm::vec3 startVec = glm::normalize(m_startDragWorld - planePoint);
        glm::vec3 currentVec = glm::normalize(currentDragWorld - planePoint);
        
        // Calculate angle between the vectors
        float angle = acos(glm::clamp(glm::dot(startVec, currentVec), -1.0f, 1.0f));

        // Determine sign of the angle
        glm::vec3 cross = glm::cross(startVec, currentVec);
        if (glm::dot(axisDir, cross) < 0) {
            angle = -angle;
        }

        if (m_snapEnabled) {
            angle = glm::radians(snapAngle(glm::degrees(angle), m_snapAngle));
        }

        glm::quat rotationChange = glm::angleAxis(angle, axisDir);
        m_target->setRotation(rotationChange * glm::quat(m_startRotation));

        return true;
    } else {
        // Update hover state
        Axis prevHovered = m_hoveredAxis;
        m_hoveredAxis = hitTestAxis(rayOrigin, rayDir);
        return m_hoveredAxis != prevHovered; // Return true if hover state changed
    }
}

bool RotateGizmo::isHovered() const {
    return m_hoveredAxis != Axis::None;
}

Axis RotateGizmo::hitTestAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const {
    if (!m_target) return Axis::None;

    float minDistance = std::numeric_limits<float>::max();
    Axis closestAxis = Axis::None;
    glm::vec3 center = m_target->getPosition();

    for (int i = 0; i < 3; ++i) {
        Axis axis = static_cast<Axis>(i);
        glm::vec3 normal = getAxisDirection(axis);
        
        // Intersect ray with the plane of the circle
        float denom = glm::dot(rayDir, normal);
        if (std::abs(denom) > 1e-6) {
            float t = glm::dot(center - rayOrigin, normal) / denom;
            if (t > 0) {
                glm::vec3 intersection = rayOrigin + t * rayDir;
                float distFromCenter = glm::length(intersection - center);
                
                // Check if the intersection is close to the circle's radius
                float distToCircle = std::abs(distFromCenter - m_size);
                if (distToCircle < 0.1f) { // Hit threshold
                    if (distToCircle < minDistance) {
                        minDistance = distToCircle;
                        closestAxis = axis;
                    }
                }
            }
        }
    }
    return closestAxis;
}


glm::vec3 RotateGizmo::getAxisDirection(Axis axis) const {
    switch (axis) {
        case Axis::X: return glm::vec3(1.0f, 0.0f, 0.0f);
        case Axis::Y: return glm::vec3(0.0f, 1.0f, 0.0f);
        case Axis::Z: return glm::vec3(0.0f, 0.0f, 1.0f);
        default: return glm::vec3(0.0f);
    }
}

glm::vec3 RotateGizmo::getColor(Axis axis) const {
    switch (axis) {
        case Axis::X: return m_xColor;
        case Axis::Y: return m_yColor;
        case Axis::Z: return m_zColor;
        default: return glm::vec3(1.0f);
    }
}

glm::vec3 RotateGizmo::getHoverColor(Axis axis) const {
    switch (axis) {
        case Axis::X: return m_xHoverColor;
        case Axis::Y: return m_yHoverColor;
        case Axis::Z: return m_zHoverColor;
        default: return glm::vec3(1.0f);
    }
}

void RotateGizmo::screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const {
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