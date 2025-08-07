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

class RotateGizmo : public Gizmo {

public:
    RotateGizmo();
    explicit RotateGizmo(std::shared_ptr<rude::Entity> target);
    ~RotateGizmo() override;

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
    void drawCircle(const glm::vec3& center, const glm::vec3& normal, float radius, const glm::vec3& color) const;
    Axis hitTestAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const;
    float distanceToCircle(const glm::vec3& point, const glm::vec3& center, const glm::vec3& normal, float radius) const;
    glm::vec3 getAxisDirection(Axis axis) const;
    glm::vec3 getColor(Axis axis) const;
    glm::vec3 getHoverColor(Axis axis) const;
    void screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const;

    std::shared_ptr<rude::Entity> m_target = nullptr;
    Axis m_activeAxis = Axis::None;
    glm::vec3 m_startRotation;
    glm::vec3 m_currentRotation;
    glm::vec3 m_startDragWorld;
    glm::vec3 m_startDragScreen;
};