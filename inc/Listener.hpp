#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <pch.h>
#include <Event.hpp>

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

#endif
