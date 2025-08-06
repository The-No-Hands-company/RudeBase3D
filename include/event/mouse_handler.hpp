#pragma once

#include "event_types.hpp"
#include "event_dispatcher.hpp"
#include <QObject>

namespace event {

class MouseHandler : public QObject {
    Q_OBJECT
public:
    MouseHandler(EventDispatcher& dispatcher);
    ~MouseHandler() override = default;

    void handleMousePress(QMouseEvent* event);
    void handleMouseRelease(QMouseEvent* event);
    void handleMouseMove(QMouseEvent* event);
    void handleWheel(QWheelEvent* event);

signals:
    void viewportUpdated();

private:
    EventDispatcher& dispatcher;
    glm::vec2 lastMousePos;
    bool isDragging = false;
};

} // namespace event 