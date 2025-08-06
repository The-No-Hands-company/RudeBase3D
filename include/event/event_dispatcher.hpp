#pragma once

#include "event_types.hpp"
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <queue>
#include <mutex>

namespace event {

class EventListener {
public:
    virtual void onEvent(const Event& e) = 0;
    virtual ~EventListener() = default;
};

class EventDispatcher {
public:
    EventDispatcher();
    using EventCallback = std::function<void(Event&)>;
    using EventQueue = std::queue<std::unique_ptr<Event>>;

    template<typename T>
    void subscribe(EventCallback callback, Priority priority = Priority::Normal) {
        std::lock_guard<std::mutex> lock(mutex);
        auto& handlers = callbacks[typeid(T)];
        handlers.push_back({priority, callback});
        // Sort handlers by priority
        std::sort(handlers.begin(), handlers.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
    }

    template<typename T>
    void dispatch(T& event) {
        std::lock_guard<std::mutex> lock(mutex);
        auto& handlers = callbacks[typeid(T)];
        for (const auto& [priority, handler] : handlers) {
            if (event.handled) break;
            handler(event);
        }
    }

    template<typename T>
    void queue(T&& event) {
        std::lock_guard<std::mutex> lock(mutex);
        eventQueue.push(std::make_unique<T>(std::move(event)));
    }

    void processQueue() {
        std::lock_guard<std::mutex> lock(mutex);
        while (!eventQueue.empty()) {
            auto event = std::move(eventQueue.front());
            eventQueue.pop();
            dispatch(*event);
        }
    }

    void clearQueue() {
        std::lock_guard<std::mutex> lock(mutex);
        while (!eventQueue.empty()) {
            eventQueue.pop();
        }
    }

    void addListener(EventListener* listener);
    void removeListener(EventListener* listener);
    void dispatch(const Event& e);

private:
    std::unordered_map<std::type_index, std::vector<std::pair<Priority, EventCallback>>> callbacks;
    EventQueue eventQueue;
    std::mutex mutex;
    std::vector<EventListener*> listeners;
};

} // namespace event 