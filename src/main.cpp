#include <pch.h>

#include <Event.hpp>
#include <EventManager.hpp>

int main() {
    EventManager<2> eventManager;
    const std::string inputData = "external input";
    eventManager.addEventListener("input", [&](const Event &event) {
        std::cout << "Name: " << event.name << " Data: " << event.data
                  << " External Input: " << inputData << std::endl;
    });
    eventManager.addEventListener("keyboard", [](const Event &event) {
        std::cout << "Name: " << event.name << " Data: " << event.data
                  << " Keyboard Event" << std::endl;
    });
    eventManager.addEventListener("mouse", [](const Event &event) {
        std::cout << "Name: " << event.name << " Data: " << event.data
                  << std::endl;
    });

    eventManager.dispatchEvent({"mouse", "x: 11, y: 99"});
    eventManager.dispatchEvent({"input", "x: 22, y: 88"});
    eventManager.dispatchEvent({"keyboard", "x: 33, y: 77"});
    eventManager.dispatchEvent({"mouse", "x: 44, y: 66"});
    eventManager.dispatchEvent({"keyboard", "x: 55, y: 55"});
    eventManager.dispatchEvent({"mouse", "x: 66, y: 44"});
    eventManager.dispatchEvent({"keyboard", "x: 77, y: 33"});
    eventManager.dispatchEvent({"keyboard", "x: 88, y: 22"});
    eventManager.dispatchEvent({"keyboard", "x: 99, y: 11"});
    eventManager.dispatchEvent({"click", "button"});

    eventManager.addEventListener("click", [](const Event &event) {
        std::cout << "Thread id: " << std::this_thread::get_id()
                  << " Event name: " << event.name
                  << " Event data: " << event.data << "\n";
    });

    for (std::size_t i = 0; i < 10000; i++) {
        eventManager.dispatchEvent(
            {"click", "button " + std::to_string(i + 1)});
    }
}
