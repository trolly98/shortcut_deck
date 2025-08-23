#pragma once

#include <Arduino.h>
#include "../button/buttons_configuration.hpp"
#include "logo.h"
class Display
{
public:
    explicit Display() = default;
    ~Display() = default;
    virtual bool init() const = 0;
    virtual void clear() const = 0;
    virtual void print(const String& text) const = 0;
    virtual void show_logo() const = 0;
    virtual void show_info() const = 0;

    virtual void print_configuration(const ButtonArray* config, 
                                     ButtonsConfiguration::index_t selected_config,
                                     ButtonsConfiguration::index_t config_count) const = 0;
};