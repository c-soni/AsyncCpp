#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <pch.h>

#include <Event.hpp>

template <std::size_t N = 1>
class EventManager {
public:
    EventManager() {
        for (auto &thread : threads) {
            thread = std::thread(&EventManager::run, this);
        }
    }

    ~EventManager() {
        shutDown = true;
        for (auto &thread : threads) {
            thread.join();
        }
    }

    auto addEventListener(std::string eventName,
        const std::function<auto(const Event &)->void> &&listener) -> void {
        registeredListeners[eventName].push_back(listener);
    }

    auto dispatchEvent(const Event &event) -> void {
        std::lock_guard<std::mutex> lock(mutex);
        eventQueue.push(event);
    }

    auto bulkProcessEvents() -> void {
        while (!eventQueue.empty()) {
            processEvent(eventQueue.front());
            eventQueue.pop();
        }
    }

private:
    std::queue<Event> eventQueue;
    std::unordered_map<std::string,
        std::vector<std::function<auto(const Event &)->void>>>
        registeredListeners;
    std::mutex mutex;
    std::array<std::thread, N> threads;
    bool shutDown = false;

    auto processEvent(const Event &event) -> void {
        if (registeredListeners.contains(event.name)) {
            for (const auto &listener : registeredListeners.at(event.name)) {
                listener(event);
            }
        }
    }

    auto run() -> void {
        std::cout << "Running thread: " << std::this_thread::get_id() << '\n';
        while (!shutDown) {
            std::lock_guard<std::mutex> lock(mutex);
            if (!eventQueue.empty()) {
                processEvent(eventQueue.front());
                eventQueue.pop();
            }
        }
    }
};

#endif
