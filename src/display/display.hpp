#pragma once

#include <Arduino.h>
#include "../button/buttons_configuration.hpp"

class Display
{
public:
    explicit Display() = default;
    ~Display() = default;
    virtual bool init() = 0;
    virtual void clear() = 0;
    virtual void print(const String& text) = 0;

    virtual void print_configuration(const ButtonArray* config, 
                                     ButtonsConfiguration::index_t selected_config,
                                     ButtonsConfiguration::index_t config_count) const = 0;
};