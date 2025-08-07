#include "gizmo/gizmo.hpp"
#include "scene/Camera.h"
#include "event/event_types.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Gizmo::Gizmo()
{
}

bool Gizmo::handleInput(const event::MouseEvent& event, const Camera& camera) {
    // For now, we'll handle events directly based on the Qt event
    if (auto* qtEvent = event.qtEvent) {
        if (qtEvent->type() == QEvent::MouseButtonPress) {
            event::MousePressEvent pressEvent(qtEvent);
            return handleMousePressed(pressEvent, camera);
        } else if (qtEvent->type() == QEvent::MouseButtonRelease) {
            event::MouseReleaseEvent releaseEvent(qtEvent);
            return handleMouseReleased(releaseEvent, camera);
        } else if (qtEvent->type() == QEvent::MouseMove) {
            event::MouseMoveEvent moveEvent(qtEvent);
            return handleMouseMoved(moveEvent, camera);
        }
    }
    return false;
}

void Gizmo::setColors(const glm::vec3& xColor, const glm::vec3& yColor, const glm::vec3& zColor) {
    m_xColor = xColor;
    m_yColor = yColor;
    m_zColor = zColor;
}

void Gizmo::setHoverColors(const glm::vec3& xColor, const glm::vec3& yColor, const glm::vec3& zColor) {
    m_xHoverColor = xColor;
    m_yHoverColor = yColor;
    m_zHoverColor = zColor;
}

void Gizmo::setSize(float size) {
    m_size = size;
}

void Gizmo::setLineWidth(float width) {
    m_lineWidth = width;
}

void Gizmo::setHoverLineWidth(float width) {
    m_hoverLineWidth = width;
}

void Gizmo::setArrowHeadStyle(ArrowHeadStyle style) {
    m_arrowHeadStyle = style;
}

void Gizmo::setShowLabels(bool showLabels) {
    m_showLabels = showLabels;
}

void Gizmo::setShowDistance(bool showDistance) {
    m_showDistance = showDistance;
}

void Gizmo::setSnapEnabled(bool enabled) {
    m_snapEnabled = enabled;
}

void Gizmo::setSnapDistance(float distance) {
    m_snapDistance = distance;
}

void Gizmo::setSnapAngle(float angle) {
    m_snapAngle = angle;
}

void Gizmo::setSnapScale(float scale) {
    m_snapScale = scale;
}

void Gizmo::drawLabel(const glm::vec3& position, const ::std::string& text, const Camera& camera) const {
    if (!m_showLabels) return;

    // TODO: Implement text rendering using a font texture or text rendering library
    // For now, this is a stub implementation that acknowledges parameter usage
    (void)position; // Future: world position for label placement
    (void)text;     // Future: text content to render
    (void)camera;   // Future: camera matrices for screen projection
}

float Gizmo::snapValue(float value, float snap) const {
    if (!m_snapEnabled) return value;
    return std::round(value / snap) * snap;
}

glm::vec3 Gizmo::snapVector(const glm::vec3& vector, float snap) const {
    if (!m_snapEnabled) return vector;
    return glm::vec3(
        snapValue(vector.x, snap),
        snapValue(vector.y, snap),
        snapValue(vector.z, snap)
    );
}

void Gizmo::screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const {
    // Convert screen coordinates to normalized device coordinates
    float ndcX = (2.0f * x) / camera.getViewportWidth() - 1.0f;
    float ndcY = 1.0f - (2.0f * y) / camera.getViewportHeight();

    // Convert to clip space
    glm::vec4 clipCoords(ndcX, ndcY, -1.0f, 1.0f);

    // Convert to eye space
    glm::vec4 eyeCoords = glm::inverse(camera.getProjectionMatrixGLM()) * clipCoords;
    eyeCoords.z = -1.0f;
    eyeCoords.w = 0.0f;

    // Convert to world space
    glm::vec4 worldCoords = glm::inverse(camera.getViewMatrixGLM()) * eyeCoords;
    outDir = glm::normalize(glm::vec3(worldCoords));
    outOrigin = camera.getPosition();
}

const glm::vec3& Gizmo::getColor(Axis axis) const {
    switch (axis) {
        case Axis::X: return m_xColor;
        case Axis::Y: return m_yColor;
        case Axis::Z: return m_zColor;
        default: return m_xColor;
    }
}

const glm::vec3& Gizmo::getHoverColor(Axis axis) const {
    switch (axis) {
        case Axis::X: return m_xHoverColor;
        case Axis::Y: return m_yHoverColor;
        case Axis::Z: return m_zHoverColor;
        default: return m_xHoverColor;
    }
}

glm::vec3 Gizmo::getAxisDirection(Axis axis) const {
    switch (axis) {
        case Axis::X: return glm::vec3(1.0f, 0.0f, 0.0f);
        case Axis::Y: return glm::vec3(0.0f, 1.0f, 0.0f);
        case Axis::Z: return glm::vec3(0.0f, 0.0f, 1.0f);
        default: return glm::vec3(0.0f);
    }
}