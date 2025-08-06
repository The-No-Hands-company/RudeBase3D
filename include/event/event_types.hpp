#pragma once

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <chrono>

namespace event {

// Event priority levels
enum class Priority {
    Highest = 0,    // Gizmo interaction
    High = 1,       // Entity selection
    Normal = 2,     // Camera control
    Low = 3,        // UI interaction
    Lowest = 4      // Background tasks
};

// Base event class
class Event {
public:
    virtual ~Event() = default;
    bool handled = false;
    Priority priority = Priority::Normal;
    std::chrono::steady_clock::time_point timestamp = std::chrono::steady_clock::now();
};

// Mouse events
class MouseEvent : public Event {
public:
    MouseEvent(QMouseEvent* qtEvent) : qtEvent(qtEvent) {}
    QMouseEvent* qtEvent;
    glm::vec2 position;
    glm::vec2 delta;
    bool isLeftButton = false;
    bool isRightButton = false;
    bool isMiddleButton = false;
    bool isCtrlPressed = false;
    bool isShiftPressed = false;
    bool isAltPressed = false;
};

class MousePressEvent : public MouseEvent {
public:
    MousePressEvent(QMouseEvent* qtEvent) : MouseEvent(qtEvent) {
        if (qtEvent) {
            position = glm::vec2(qtEvent->position().x(), qtEvent->position().y());
            isLeftButton = qtEvent->button() == Qt::LeftButton;
            isRightButton = qtEvent->button() == Qt::RightButton;
            isMiddleButton = qtEvent->button() == Qt::MiddleButton;
            isCtrlPressed = qtEvent->modifiers() & Qt::ControlModifier;
            isShiftPressed = qtEvent->modifiers() & Qt::ShiftModifier;
            isAltPressed = qtEvent->modifiers() & Qt::AltModifier;
        }
    }
};

class MouseReleaseEvent : public MouseEvent {
public:
    MouseReleaseEvent(QMouseEvent* qtEvent) : MouseEvent(qtEvent) {
        if (qtEvent) {
            position = glm::vec2(qtEvent->position().x(), qtEvent->position().y());
            isLeftButton = qtEvent->button() == Qt::LeftButton;
            isRightButton = qtEvent->button() == Qt::RightButton;
            isMiddleButton = qtEvent->button() == Qt::MiddleButton;
            isCtrlPressed = qtEvent->modifiers() & Qt::ControlModifier;
            isShiftPressed = qtEvent->modifiers() & Qt::ShiftModifier;
            isAltPressed = qtEvent->modifiers() & Qt::AltModifier;
        }
    }
};

class MouseMoveEvent : public MouseEvent {
public:
    MouseMoveEvent(QMouseEvent* qtEvent) : MouseEvent(qtEvent) {
        if (qtEvent) {
            position = glm::vec2(qtEvent->position().x(), qtEvent->position().y());
            isLeftButton = qtEvent->buttons() & Qt::LeftButton;
            isRightButton = qtEvent->buttons() & Qt::RightButton;
            isMiddleButton = qtEvent->buttons() & Qt::MiddleButton;
            isCtrlPressed = qtEvent->modifiers() & Qt::ControlModifier;
            isShiftPressed = qtEvent->modifiers() & Qt::ShiftModifier;
            isAltPressed = qtEvent->modifiers() & Qt::AltModifier;
        }
    }
};

class MouseWheelEvent : public Event {
public:
    MouseWheelEvent(QWheelEvent* qtEvent) : qtEvent(qtEvent) {}
    QWheelEvent* qtEvent;
    float delta;
    bool isCtrlPressed = false;
    bool isShiftPressed = false;
    bool isAltPressed = false;
};

// Keyboard events
class KeyEvent : public Event {
public:
    KeyEvent(QKeyEvent* qtEvent) : qtEvent(qtEvent) {}
    QKeyEvent* qtEvent;
    int key;
    bool isPressed;
    bool isRepeated;
    bool isCtrlPressed = false;
    bool isShiftPressed = false;
    bool isAltPressed = false;
};

// Viewport specific events
class ViewportResizeEvent : public Event {
public:
    ViewportResizeEvent(int width, int height) : width(width), height(height) {}
    int width;
    int height;
};

class SelectionChangedEvent : public Event {
public:
    SelectionChangedEvent(void* selected) : selected(selected) {}
    void* selected;
};

// Gizmo specific events
class GizmoEvent : public Event {
public:
    GizmoEvent() { priority = Priority::Highest; }
    enum class Type {
        Hover,
        DragStart,
        Drag,
        DragEnd,
        Click
    };
    Type type;
    glm::vec3 worldPosition;
    glm::vec3 worldDelta;
    bool isActive = false;
};

// Camera specific events
class CameraEvent : public Event {
public:
    CameraEvent() { priority = Priority::Normal; }
    enum class Type {
        Orbit,
        Pan,
        Zoom,
        Reset
    };
    Type type;
    glm::vec2 delta;
    float zoomDelta = 0.0f;
};

// Entity specific events
class EntityEvent : public Event {
public:
    EntityEvent() { priority = Priority::High; }
    enum class Type {
        Created,
        Deleted,
        Modified,
        Selected,
        Deselected
    };
    Type type;
    void* entity;
    glm::mat4 transform;
};

// Tool specific events
class ToolEvent : public Event {
public:
    ToolEvent() { priority = Priority::High; }
    enum class Type {
        Activated,
        Deactivated,
        Changed
    };
    Type type;
    std::string toolName;
};

} // namespace event 