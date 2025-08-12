#pragma once

#include "button.hpp"
#include <Keyboard.h>

#include "../parsers/key_parser.hpp"
#include "../parsers/special_key_factory.hpp"
#include "../converters/keyboard_key_converter.hpp"

#define MAX_SPECIAL_KEYS 2

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
    BTN_9,
    UNKNOWN
  };

  FunctionButton(Number number, 
                 const String &key);
  FunctionButton();
  ~FunctionButton();

  const Number number() const;
  const String &key() const;

  static Number get_number(unsigned int value);
  static String get_number_string(Number number);

protected:
  virtual void pressed() override;
  virtual void released() override;

private:
  typedef void (*KeyboardStringAction)(const String &);
  typedef void (*KeyboardIntAction)(uint8_t);

  void _action(KeyboardStringAction write_function,
               KeyboardIntAction special_function);

  static pin_number_t _get_attached_pin(Number number);

  String _key;
  Number _number;
  SpecialKey* _special_key[MAX_SPECIAL_KEYS];
  int _special_key_count;
  KeyParser _key_parser;
  static KeyboardKeyConverter* _keyboard_converter;

};
