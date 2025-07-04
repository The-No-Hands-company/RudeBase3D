#include "event/event_dispatcher.hpp"
#include <QDebug>

namespace event {

EventDispatcher::EventDispatcher() {
    qDebug() << "[EventDispatcher::ctor] Created EventDispatcher at" << this;
}

void EventDispatcher::addListener(EventListener* listener) {
    qDebug() << "[EventDispatcher::addListener] Adding listener" << listener << "to dispatcher" << this;
    listeners.push_back(listener);
}

void EventDispatcher::removeListener(EventListener* listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void EventDispatcher::dispatch(const Event& e) {
    qDebug() << "[EventDispatcher::dispatch] Dispatching event of type:" << typeid(e).name() << ", listeners count:" << listeners.size();
    for (auto* listener : listeners) {
        qDebug() << "[EventDispatcher::dispatch] Notifying listener:" << listener;
        listener->onEvent(e);
    }
}

} // namespace event 