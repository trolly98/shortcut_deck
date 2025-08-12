#pragma once

#include "../parsers/special_key.hpp"

class KeyboardKeyConverter
{
public:
    virtual int operator()(const SpecialKey &key) const = 0;
};
