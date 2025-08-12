#pragma once

#include "../utils.hpp"
#include "../configs.hpp"
#include <Arduino.h>

constexpr uint8_t DEBOUNCE_TIME_MS = 30;

class Button 
{
public:
  using pin_number_t = uint8_t;
  Button();
  Button(const Button& button);
  Button(pin_number_t pin);

  void press(bool state);
  void update();

  const pin_number_t pin() const;
  const bool is_valid() const;

protected:
  virtual void pressed() = 0;
  virtual void released() = 0;

private:
  pin_number_t _pin;
  unsigned long _last_pressed_time;
  bool _pressed;
};