#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>

struct Event
{
    std::string name;
    std::string data;
    std::size_t moreData;

    Event(std::string name, std::string data, std::size_t moreData = 1) : name(name), data(data), moreData(moreData) {}
    Event(const Event &other) = delete;
    Event(const Event &&other) = delete;
};

class Listener
{
    std::function<auto(const Event &)->void> handler;

public:
    Listener(std::function<void(const Event &)> handler) : handler(handler) {}

    auto operator()(const Event &data) const -> void
    {
        handler(data);
    }
};

template<unsigned int N = 1>
class EventManager
{
    std::queue <Event> eventQueue;
    std::unordered_map<std::string, std::vector<Listener>> registeredListeners;
    std::mutex mutex;
    std::array<std::thread, N> threads;
    bool shutDown = false;

public:

    EventManager() {
        // thread = std::thread(&EventManager::run, this);
        for (unsigned int i = 0; i < N; i++)
        {
            threads[i] = std::thread(&EventManager::run, this);
        }
    }

    ~EventManager() {
        shutDown = true;
        for (unsigned int i = 0; i < N; i++)
        {
            threads[i].join();
        }
    }

    auto addEventListener(std::string eventName, const Listener &&listener) -> void
    {
        registeredListeners[eventName].push_back(listener);
    }

    auto processEvent(const Event &event) -> void
    {
        if (registeredListeners.contains(event.name))
        {
            for (const auto &listener : registeredListeners.at(event.name))
            {
                listener(event);
            }
        }
    }

    auto dispatchEvent(const std::string & name, const std::string & data) -> void
    {
        std::lock_guard<std::mutex> lock(mutex);
        eventQueue.emplace(name, data);
        // signal thread of availability - using condition variable?
    }

    auto run() -> void
    {
        std::cout << "Running thread: " << std::this_thread::get_id() << "\n";
        while (!shutDown)
        {
            if (!eventQueue.empty())
            {
                std::lock_guard<std::mutex> lock(mutex);
                processEvent(eventQueue.front());
                eventQueue.pop();
            }
        }
    }

    auto bulkProcessEvents() -> void
    {
        while (!eventQueue.empty())
        {
            processEvent(eventQueue.front());
            eventQueue.pop();
        }
    }
};

int main()
{
    EventManager<2> eventManager;
    const std::string inputData = "external input";
    eventManager.addEventListener("input", Listener([&](const Event & event) {
        std::cout << "Name: " << event.name << " Data: " << event.data << " External Input: " << inputData << std::endl;
    }));
    eventManager.addEventListener("keyboard", Listener([](const Event & event) {
        std::cout << "Name: " << event.name << " Data: " << event.data << " Keyboard Event" << std::endl;
    }));
    eventManager.addEventListener("mouse", Listener([](const Event & event) {
        std::cout << "Name: " << event.name << " Data: " << event.data << std::endl;
    }));


    eventManager.dispatchEvent("mouse", "x: 11, y: 99");
    eventManager.dispatchEvent("input", "x: 22, y: 88");
    eventManager.dispatchEvent("keyboard", "x: 33, y: 77");
    eventManager.dispatchEvent("mouse", "x: 44, y: 66");
    eventManager.dispatchEvent("keyboard", "x: 55, y: 55");
    eventManager.dispatchEvent("mouse", "x: 66, y: 44");
    eventManager.dispatchEvent("keyboard", "x: 77, y: 33");
    eventManager.dispatchEvent("keyboard", "x: 88, y: 22");
    eventManager.dispatchEvent("keyboard", "x: 99, y: 11");
    eventManager.dispatchEvent("click", "button");

    eventManager.addEventListener("click", Listener([](const Event & event) {
        std::cout << "Thread id: " << std::this_thread::get_id() << " Event name:" << event.name << "\n";
    }));

    for (std::size_t i = 0; i < 1000; i++)
    {
        eventManager.dispatchEvent("click", "button " + std::to_string(i + 1));
    }
}
