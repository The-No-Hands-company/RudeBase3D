#pragma once

#include "gizmo/translate_gizmo.hpp"
#include "gizmo/rotate_gizmo.hpp"
#include "gizmo/scale_gizmo.hpp"
#include "core/entity.hpp"
#include "event/event_types.hpp"
#include <memory>

class Camera;

// Forward declare specific event types
namespace event {
    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
    class MouseMovedEvent;
}

enum class GizmoType {
    None,
    Translate,
    Rotate,
    Scale
};

class GizmoManager {
public:
    GizmoManager();

    void setActiveGizmo(GizmoType type);
    void setSelectedEntity(std::shared_ptr<Entity> entity);

    // Event handling
    bool handleMousePress(const event::MousePressEvent& e, const Camera& camera);
    bool handleMouseMove(const event::MouseMoveEvent& e, const Camera& camera);
    bool handleMouseRelease(const event::MouseReleaseEvent& e, const Camera& camera);

    void draw(const Camera& camera) const;

private:
    std::shared_ptr<Entity> m_selectedEntity;
    GizmoType m_activeGizmoType = GizmoType::None;

    std::unique_ptr<TranslateGizmo> m_translateGizmo;
    std::unique_ptr<RotateGizmo> m_rotateGizmo;
    std::unique_ptr<ScaleGizmo> m_scaleGizmo;

    Gizmo* m_activeGizmo = nullptr;
};
