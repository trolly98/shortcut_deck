#pragma once

#include "display.hpp"

class DisplayManager
{
public:
    explicit DisplayManager();
    ~DisplayManager();
    bool init();
    const Display* display() const;

private:
    Display* _display;
};