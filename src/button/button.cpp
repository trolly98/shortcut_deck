#include "button.hpp"

Button::Button(pin_number_t pin, const name_t& name) : 
  _pin(pin),
  _name(name),
  _last_pressed_time{0},
  _pressed{false},
  _long_pressed_time{0},
  _long_pressed{false}
{
  if (!this->is_valid())
  {
    Serial.print(F("Button is invalid! Pin: "));
    Serial.println(_pin);
    return;
  }
  pinMode(this->pin(), INPUT_PULLUP);
}

Button::Button() : Button(0, "") {}

Button::Button(const Button& button) : Button(button._pin, button._name) {}


void Button::_press(bool state)
{
    if ((millis() - _last_pressed_time <= DEBOUNCE_TIME_MS))
    {
        return;
    }

    if (state == _pressed)
    {
      if (!state)
      {
        return;
      }
      
      if (_long_pressed_time == 0)
      {
          _long_pressed_time = millis();
          _long_pressed = false;
      }

      if (!_long_pressed && (millis() - _long_pressed_time >= LONG_PRESS_THRESHOLD_MS))
      {
          Serial.println("LONG PRESS");
          _long_pressed = true;
          this->long_pressed();
      }
      return;
    }

    _last_pressed_time = millis();

    if (state)
    {
        this->pressed();
    }
    else
    {
        if (!_long_pressed)
        {
          this->released();
        }
        _long_pressed = false;
        _long_pressed_time = 0;
    }

    _pressed = state;
}

void Button::update()
{
  if (!this->is_valid())
  {
    return;
  }
  this->_press(!digitalRead(_pin));
}

void Button::set_name(const name_t& name)
{
  _name = name;
}

const Button::pin_number_t Button::pin() const
{
  return _pin;
}

const Button::name_t& Button::name() const
{
  return _name;
}

const bool Button::is_valid() const
{
  return _pin != 0;
}