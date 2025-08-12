#include "button.hpp"

Button::Button(pin_number_t pin) : 
  _pin(pin),
  _last_pressed_time{0},
  _pressed{false}
{
  if (!this->is_valid())
  {
    return;
  }
  pinMode(this->pin(), INPUT_PULLUP);
}

Button::Button() : Button(0) {}

Button::Button(const Button& button) : Button(button._pin) {}


void Button::press(bool state)
{
  if(state == _pressed || (millis() - _last_pressed_time <= DEBOUNCE_TIME_MS))
  {
    return;
  }
  _last_pressed_time = millis();
  if (state)
  {
    this->pressed();
  }
  else
  {
    this->released();
  }
  _pressed = state;
}

void Button::update()
{
  if (!this->is_valid())
  {
    return;
  }
  this->press(!digitalRead(_pin));
}

const Button::pin_number_t Button::pin() const
{
  return _pin;
}

const bool Button::is_valid() const
{
  return _pin != 0;
}