#pragma once
#include "Keyboard.h"

constexpr uint8_t DEBOUNCE_TIME_MS = 30;
constexpr uint8_t MAX_BTN_NUMBER = 8;

class Button 
{
public:
  using pin_number_t = uint8_t;
  Button() : Button(0) {};
  Button(const Button& button) : Button(button._pin) {}
  Button(pin_number_t pin) : 
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

  void press(bool state)
  {
    if(state == _pressed || (millis() - _last_pressed_time  <= DEBOUNCE_TIME_MS))
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
  void update()
  {
    if (!this->is_valid())
    {
      return;
    }
    this->press(!digitalRead(_pin));
  }

  const pin_number_t pin() const { return _pin; }
  const bool is_valid() const { return _pin!=0; }

protected:
  virtual void pressed() = 0;
  virtual void released() = 0;

private:
  pin_number_t _pin;
  unsigned long _last_pressed_time;
  bool _pressed;
};

class FunctionButton : public Button
{
public:
  enum class Number
  {
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4,
    BTN_5,
    BTN_6,
    BTN_7,
    BTN_8,
    UNKNOWN
  };

  FunctionButton() : FunctionButton(Number::UNKNOWN, ""){}
  FunctionButton(Number number, const String &key) : 
    Button(FunctionButton::_get_attached_pin(number)),
    _key(key),
    _number{number}
  {
    if (!this->is_valid())
    {
      Serial.println("Button with pin: " + String(this->pin()) + " - and key: " + key + " is invalid");
    }
    else
    {
      Serial.println("Button with pin: " + String(this->pin()) + " - and key: " + key + " created");
    }
  }

  const Number number() const
  {
    return _number;
  }

  const String &key() const
  {
    return _key;
  }

  static Number get_number(unsigned int value)
  {
    if (value < 0 || value > (MAX_BTN_NUMBER-1))
    {
      return Number::UNKNOWN;
    }
    else 
    {
      return static_cast<Number>(value);
    }
  }
  static String get_number_string(Number number)
  {
    switch(number)
    {
      case Number::BTN_1:
        return "BTN_1";
      case Number::BTN_2:
        return "BTN_2";
      case Number::BTN_3:
        return "BTN_3";
      case Number::BTN_4:
        return "BTN_4";
      case Number::BTN_5:
        return "BTN_5";
      case Number::BTN_6:
        return "BTN_6";
      case Number::BTN_7:
        return "BTN_7";
      case Number::BTN_8:
        return "BTN_8";
      default:
        return "UNKNOWN";
    }
  }

protected:
  virtual void pressed() override
  {
    if (_key.length() > 1)
    {
      Keyboard.print(_key);
    }
    else if (_key.length() > 0)
    {
      Keyboard.press(_key.charAt(0));
    }
    Serial.println("Pressed " + _key);
  }
  virtual void released() override
  {
    if (_key.length() > 1)
    {
      //
    }
    else if (_key.length() > 0)
    {
      Keyboard.release(_key.charAt(0));
    }
    Serial.println("Release " + String(_key));
    //Keyboard.release(_key);
  }
private:
  static pin_number_t _get_attached_pin(Number number)
  {
    switch (number)
    {
      case Number::BTN_1:
        return 2;
      default:
        return 0;
    }
  }
  String _key;
  Number _number;
};