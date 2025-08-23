#pragma once

#include "../utils.hpp"
#include "../configs.hpp"
#include <Arduino.h>

constexpr uint8_t DEBOUNCE_TIME_MS = 30;
constexpr uint32_t LONG_PRESS_THRESHOLD_MS = 1000;

class Button 
{
public:
  using pin_number_t = uint8_t;
  Button();
  Button(const Button& button);
  Button(pin_number_t pin);

  void update();

  const pin_number_t pin() const;
  const bool is_valid() const;

protected:
  virtual void pressed() = 0;
  virtual void long_pressed() = 0;
  virtual void released() = 0;

private:
  void _press(bool state);
  pin_number_t _pin;
  unsigned long _last_pressed_time;
  unsigned long _long_pressed_time;
  bool _long_pressed;
  bool _pressed;
};