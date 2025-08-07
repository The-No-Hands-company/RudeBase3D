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

class TranslateGizmo : public Gizmo {

public:
    TranslateGizmo();
    explicit TranslateGizmo(std::shared_ptr<rude::Entity> target);
    ~TranslateGizmo() override;

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
    void drawArrow(const glm::vec3& start, const glm::vec3& direction, const glm::vec3& color, float size) const;
    void drawConeArrowHead(const glm::vec3& position, const glm::vec3& direction, float size, const glm::vec3& color) const;
    void drawCubeArrowHead(const glm::vec3& position, const glm::vec3& direction, float size, const glm::vec3& color) const;
    void drawPyramidArrowHead(const glm::vec3& position, const glm::vec3& direction, float size, const glm::vec3& color) const;
    void drawAxisLabel(const glm::vec3& position, const glm::vec3& direction, const std::string& label, const glm::vec3& color) const;
    Axis hitTestAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const;
    float distanceToLine(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& lineStart, const glm::vec3& lineDir, float lineLength) const;
    float distanceToArrowHead(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& position, const glm::vec3& direction, float size) const;
    float distanceToCone(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& position, const glm::vec3& direction, float size) const;
    float distanceToCube(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& position, const glm::vec3& direction, float size) const;
    float distanceToSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& position, float size) const;
    glm::vec3 getAxisDirection(Axis axis) const;
    glm::vec3 getColor(Axis axis) const;
    glm::vec3 getHoverColor(Axis axis) const;
    void screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const;

    std::shared_ptr<rude::Entity> m_target = nullptr;
    glm::vec3 m_dragStartPosition;
    glm::vec3 m_dragAxis;
    float m_dragOffset = 0.0f;
};