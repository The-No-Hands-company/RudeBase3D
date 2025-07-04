#pragma once

#include "event_types.hpp"
#include "event_dispatcher.hpp"
#include <QObject>
#include <unordered_set>

namespace event {

class KeyboardHandler : public QObject {
    Q_OBJECT
public:
    KeyboardHandler(EventDispatcher& dispatcher);
    ~KeyboardHandler() override = default;

    void handleKeyPress(QKeyEvent* event);
    void handleKeyRelease(QKeyEvent* event);
    bool isKeyPressed(int key) const;

private:
    EventDispatcher& dispatcher;
    std::unordered_set<int> pressedKeys;
};

} // namespace event 