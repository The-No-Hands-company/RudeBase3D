#pragma once

#include "gizmo.hpp"
#include <glm/glm.hpp>
#include <memory>

class Entity;
class Camera;
namespace event {
    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
    class MouseMovedEvent;
}

class ScaleGizmo : public Gizmo {

public:
    ScaleGizmo();
    explicit ScaleGizmo(std::shared_ptr<rude::Entity> target);
    ~ScaleGizmo() override;

    void setTarget(std::shared_ptr<rude::Entity> target) { m_target = target; }
    std::shared_ptr<rude::Entity> getTarget() const { return m_target; }

    void draw(const Camera& camera) const override;
    
    // Specific event handlers
    bool handleMousePressed(const event::MousePressEvent& e, const Camera& camera) override;
    bool handleMouseReleased(const event::MouseReleaseEvent& e, const Camera& camera) override;
    bool handleMouseMoved(const event::MouseMoveEvent& e, const Camera& camera) override;
    
    bool isHovered() const override;
    bool isActive() const override { return m_isDragging; }

private:
    void drawCube(const glm::vec3& position, const glm::vec3& direction, float size, const glm::vec3& color) const;
    Axis hitTestAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const;
    float distanceToCube(const glm::vec3& point, const glm::vec3& center, const glm::vec3& direction, float size) const;
    void screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const;

    std::shared_ptr<rude::Entity> m_target = nullptr;
    Axis m_activeAxis = Axis::None;
    glm::vec3 m_startScale;
    glm::vec3 m_currentScale;
    glm::vec3 m_startDragPosition;
};