#include "event/keyboard_handler.hpp"
#include <QKeyEvent>

namespace event {

KeyboardHandler::KeyboardHandler(EventDispatcher& dispatcher) : dispatcher(dispatcher) {}

void KeyboardHandler::handleKeyPress(QKeyEvent* event) {
    KeyEvent keyEvent(event);
    keyEvent.key = event->key();
    keyEvent.isPressed = true;
    keyEvent.isRepeated = event->isAutoRepeat();
    
    pressedKeys.insert(event->key());
    
    dispatcher.dispatch(keyEvent);
}

void KeyboardHandler::handleKeyRelease(QKeyEvent* event) {
    KeyEvent keyEvent(event);
    keyEvent.key = event->key();
    keyEvent.isPressed = false;
    keyEvent.isRepeated = event->isAutoRepeat();
    
    pressedKeys.erase(event->key());
    
    dispatcher.dispatch(keyEvent);
}

bool KeyboardHandler::isKeyPressed(int key) const {
    return pressedKeys.find(key) != pressedKeys.end();
}

} // namespace event 
