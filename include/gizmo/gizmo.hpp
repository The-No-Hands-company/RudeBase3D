#pragma once

#include "scene/Camera.h"
#include "event/event_types.hpp"
#include <glm/glm.hpp>
#include <string>

namespace rude {
    class Entity;
}
namespace event {
    class MousePressEvent;
    class MouseReleaseEvent;
    class MouseMoveEvent;
}

enum class Axis {
    None = -1,
    X = 0,
    Y = 1,
    Z = 2
};

enum class ArrowHeadStyle {
    Cone,
    Cube,
    Sphere
};

class Gizmo {

public:
    explicit Gizmo();
    virtual ~Gizmo() = default;
    virtual void draw(const Camera& camera) const = 0;
    
    // Generic event handler - delegates to specific handlers
    bool handleInput(const event::MouseEvent& event, const Camera& camera);
    
    // Specific event handlers
    virtual bool handleMousePressed(const event::MousePressEvent& e, const Camera& camera) { 
        // Base implementation - record event for potential subclass usage
        (void)e; (void)camera; 
        return false; 
    }
    virtual bool handleMouseReleased(const event::MouseReleaseEvent& e, const Camera& camera) { 
        // Base implementation - record event for potential subclass usage
        (void)e; (void)camera; 
        return false; 
    }
    virtual bool handleMouseMoved(const event::MouseMoveEvent& e, const Camera& camera) { 
        // Base implementation - record event for potential subclass usage
        (void)e; (void)camera; 
        return false; 
    }
    
    virtual bool isHovered() const = 0;
    virtual bool isActive() const = 0;

    // Target entity management
    virtual void setTarget(std::shared_ptr<rude::Entity> target) = 0;
    virtual std::shared_ptr<rude::Entity> getTarget() const = 0;

    // Visual feedback settings
    void setColors(const glm::vec3& xColor, const glm::vec3& yColor, const glm::vec3& zColor);
    void setHoverColors(const glm::vec3& xColor, const glm::vec3& yColor, const glm::vec3& zColor);
    void setSize(float size);
    void setLineWidth(float width);
    void setHoverLineWidth(float width);

    // Snapping settings
    void setSnapEnabled(bool enabled);
    void setSnapDistance(float distance);
    void setSnapAngle(float angle);
    void setSnapScale(float scale);

    // Style settings
    void setArrowHeadStyle(ArrowHeadStyle style);
    void setShowLabels(bool showLabels);
    void setShowDistance(bool showDistance);

protected:
    void drawLabel(const glm::vec3& position, const std::string& text, const Camera& camera) const;
    
    // Utility methods for derived classes
    float snapValue(float value, float snap) const;
    glm::vec3 snapVector(const glm::vec3& vector, float snap) const;
    void screenToWorldRay(float x, float y, const Camera& camera, glm::vec3& outOrigin, glm::vec3& outDir) const;
    const glm::vec3& getColor(Axis axis) const;
    const glm::vec3& getHoverColor(Axis axis) const;
    glm::vec3 getAxisDirection(Axis axis) const;

    glm::vec3 m_xColor{1.0f, 0.0f, 0.0f};
    glm::vec3 m_yColor{0.0f, 1.0f, 0.0f};
    glm::vec3 m_zColor{0.0f, 0.0f, 1.0f};
    glm::vec3 m_xHoverColor{1.0f, 1.0f, 0.0f};
    glm::vec3 m_yHoverColor{1.0f, 1.0f, 0.0f};
    glm::vec3 m_zHoverColor{1.0f, 1.0f, 0.0f};

    float m_size = 1.0f;
    float m_lineWidth = 2.0f;
    float m_hoverLineWidth = 4.0f;

    bool m_snapEnabled = false;
    float m_snapDistance = 0.1f;
    float m_snapAngle = 15.0f; // degrees
    float m_snapScale = 0.1f;

    ArrowHeadStyle m_arrowHeadStyle = ArrowHeadStyle::Cone;
    bool m_showLabels = true;
    bool m_showDistance = false;

    Axis m_hoveredAxis = Axis::None;
    bool m_isDragging = false;
};