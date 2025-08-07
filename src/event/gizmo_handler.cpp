#include "event/gizmo_handler.hpp"
#include "gizmo/gizmo.hpp"
#include "gizmo/translate_gizmo.hpp"
#include "gizmo/rotate_gizmo.hpp"
#include "gizmo/scale_gizmo.hpp"
#include "scene/Camera.h"
#include <QMouseEvent>

GizmoHandler::GizmoHandler(QObject* parent)
    : QObject(parent)
    , m_activeGizmo(nullptr)
    , m_camera(nullptr)
{
}

void GizmoHandler::setCamera(Camera* camera) {
    m_camera = camera;
}

void GizmoHandler::handleMousePress(QMouseEvent* event) {
    if (!m_camera || !m_activeGizmo) return;

    if (m_activeGizmo->handleInput(event, *m_camera)) {
        event->accept();
        emit gizmoInteractionStarted();
        return;
    }
    event->ignore();
}

void GizmoHandler::handleMouseMove(QMouseEvent* event) {
    if (!m_camera || !m_activeGizmo) return;

    bool wasHovering = m_activeGizmo->isHovered();
    if (m_activeGizmo->handleInput(event, *m_camera)) {
        event->accept();
        bool isHovering = m_activeGizmo->isHovered();
        if (wasHovering != isHovering) {
            emit gizmoHoverChanged(isHovering);
        }
        return;
    }
    event->ignore();
}

void GizmoHandler::handleMouseRelease(QMouseEvent* event) {
    if (!m_camera || !m_activeGizmo) return;

    if (m_activeGizmo->handleInput(event, *m_camera)) {
        event->accept();
        emit gizmoInteractionEnded();
        return;
    }
    event->ignore();
}

bool GizmoHandler::isHovered(int x, int y) const {
    if (!m_camera || !m_activeGizmo) return false;
    
    // x, y would be used for precise mouse position hit testing
    // For now, delegate to gizmo's isHovered method
    (void)x; // Future: screen coordinates for hit testing
    (void)y; // Future: screen coordinates for hit testing
    
    return m_activeGizmo->isHovered();
}

void GizmoHandler::setActiveGizmo(Gizmo* gizmo) {
    m_activeGizmo = gizmo;
}

Gizmo* GizmoHandler::getActiveGizmo() const {
    return m_activeGizmo;
} 
