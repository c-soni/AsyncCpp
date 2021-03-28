#ifndef EVENT_HPP
#define EVENT_HPP

#include <pch.h>

struct Event
{
    std::string name;
    std::string data;

    Event(std::string name, std::string data) : name(name), data(data) {}
};

#endif
