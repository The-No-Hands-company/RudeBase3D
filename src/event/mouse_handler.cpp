#include "event/event_types.hpp"
#include "event/mouse_handler.hpp"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

namespace event {

MouseHandler::MouseHandler(EventDispatcher& dispatcher) : dispatcher(dispatcher) {
    qDebug() << "[MouseHandler::ctor] MouseHandler created with dispatcher at" << &dispatcher;
}

void MouseHandler::handleMousePress(QMouseEvent* event) {
    qDebug() << "[MouseHandler] handleMousePress called";
    MousePressEvent pressEvent(event);
    pressEvent.position = glm::vec2(event->pos().x(), event->pos().y());
    pressEvent.isLeftButton = event->button() == Qt::LeftButton;
    pressEvent.isRightButton = event->button() == Qt::RightButton;
    pressEvent.isMiddleButton = event->button() == Qt::MiddleButton;
    pressEvent.isCtrlPressed = event->modifiers() & Qt::ControlModifier;
    pressEvent.isShiftPressed = event->modifiers() & Qt::ShiftModifier;
    pressEvent.isAltPressed = event->modifiers() & Qt::AltModifier;
    
    lastMousePos = pressEvent.position;
    isDragging = true;
    
    dispatcher.dispatch(static_cast<Event&>(pressEvent));
    emit viewportUpdated();
}

void MouseHandler::handleMouseRelease(QMouseEvent* event) {
    qDebug() << "[MouseHandler] handleMouseRelease called";
    MouseReleaseEvent releaseEvent(event);
    releaseEvent.position = glm::vec2(event->pos().x(), event->pos().y());
    releaseEvent.isLeftButton = event->button() == Qt::LeftButton;
    releaseEvent.isRightButton = event->button() == Qt::RightButton;
    releaseEvent.isMiddleButton = event->button() == Qt::MiddleButton;
    releaseEvent.isCtrlPressed = event->modifiers() & Qt::ControlModifier;
    releaseEvent.isShiftPressed = event->modifiers() & Qt::ShiftModifier;
    releaseEvent.isAltPressed = event->modifiers() & Qt::AltModifier;
    
    isDragging = false;
    
    dispatcher.dispatch(static_cast<Event&>(releaseEvent));
    emit viewportUpdated();
}

void MouseHandler::handleMouseMove(QMouseEvent* event) {
    qDebug() << "[MouseHandler] handleMouseMove called";
    MouseMoveEvent moveEvent(event);
    moveEvent.position = glm::vec2(event->pos().x(), event->pos().y());
    moveEvent.delta = moveEvent.position - lastMousePos;
    moveEvent.isLeftButton = event->buttons() & Qt::LeftButton;
    moveEvent.isRightButton = event->buttons() & Qt::RightButton;
    moveEvent.isMiddleButton = event->buttons() & Qt::MiddleButton;
    moveEvent.isCtrlPressed = event->modifiers() & Qt::ControlModifier;
    moveEvent.isShiftPressed = event->modifiers() & Qt::ShiftModifier;
    moveEvent.isAltPressed = event->modifiers() & Qt::AltModifier;
    
    lastMousePos = moveEvent.position;
    
    dispatcher.dispatch(static_cast<Event&>(moveEvent));
    emit viewportUpdated();
}

void MouseHandler::handleWheel(QWheelEvent* event) {
    qDebug() << "[MouseHandler] handleWheel called";
    MouseWheelEvent wheelEvent(event);
    wheelEvent.delta = event->angleDelta().y() / 120.0f; // Convert to standard scroll units
    wheelEvent.isCtrlPressed = event->modifiers() & Qt::ControlModifier;
    wheelEvent.isShiftPressed = event->modifiers() & Qt::ShiftModifier;
    wheelEvent.isAltPressed = event->modifiers() & Qt::AltModifier;
    
    dispatcher.dispatch(static_cast<Event&>(wheelEvent));
    emit viewportUpdated();
}

} // namespace event 