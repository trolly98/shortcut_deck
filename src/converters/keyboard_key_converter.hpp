#pragma once

#include "../parsers/special_key.hpp"

class KeyboardKeyConverter
{
public:
    virtual void press(uint8_t key) = 0;
    virtual void release(uint8_t key) = 0;
    virtual void print(const String& s) = 0;
    virtual int operator()(const SpecialKey &key) const = 0;
};
