#include "gizmo/gizmo_manager.hpp"
#include "scene/Camera.h"
#include "event/event_types.hpp"

GizmoManager::GizmoManager() {
    m_translateGizmo = std::make_unique<TranslateGizmo>();
    m_rotateGizmo = std::make_unique<RotateGizmo>();
    m_scaleGizmo = std::make_unique<ScaleGizmo>();
}

void GizmoManager::setActiveGizmo(GizmoType type) {
    m_activeGizmoType = type;
    switch (type) {
        case GizmoType::Translate:
            m_activeGizmo = m_translateGizmo.get();
            break;
        case GizmoType::Rotate:
            m_activeGizmo = m_rotateGizmo.get();
            break;
        case GizmoType::Scale:
            m_activeGizmo = m_scaleGizmo.get();
            break;
        default:
            m_activeGizmo = nullptr;
            break;
    }
    if (m_activeGizmo) {
        m_activeGizmo->setTarget(m_selectedEntity);
    }
}

void GizmoManager::setSelectedEntity(std::shared_ptr<rude::Entity> entity) {
    m_selectedEntity = entity;
    if (m_activeGizmo) {
        m_activeGizmo->setTarget(m_selectedEntity);
    }
}

bool GizmoManager::handleMousePress(const event::MousePressEvent& e, const Camera& camera) {
    if (m_activeGizmo && m_selectedEntity) {
        return m_activeGizmo->handleMousePressed(e, camera);
    }
    return false;
}

bool GizmoManager::handleMouseMove(const event::MouseMoveEvent& e, const Camera& camera) {
    if (m_activeGizmo && m_selectedEntity) {
        return m_activeGizmo->handleMouseMoved(e, camera);
    }
    return false;
}

bool GizmoManager::handleMouseRelease(const event::MouseReleaseEvent& e, const Camera& camera) {
    if (m_activeGizmo && m_selectedEntity) {
        return m_activeGizmo->handleMouseReleased(e, camera);
    }
    return false;
}

void GizmoManager::draw(const Camera& camera) const {
    if (m_activeGizmo && m_selectedEntity) {
        m_activeGizmo->draw(camera);
    }
}
