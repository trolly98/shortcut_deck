#include "function_button.hpp"
#include "PIN_CONFIGS.h"
#include "../converters/arduino_keyboard_key_converter.hpp"


KeyboardKeyConverter* FunctionButton::_keyboard_converter = new ArduinoKeyboardKeyConverter();

FunctionButton::FunctionButton(Number number, 
                               const String &key,
                               const String &name) : 
    Button(FunctionButton::_get_attached_pin(number), name),
    _key(key),
    _number{number},
    _special_key_count{0},
    _key_parser{}
{
    if (!this->is_valid() || key == "" || number == Number::UNKNOWN)
    {
        ////Serial.println("Button with pin: " + String(this->pin()) + " - and key: " + key + " is invalid");
        return;
    }
    else
    {
        //Serial.println("Button with pin: " + String(this->pin()) + " - and key: " + key + " created");
    }
    this->set_key(_key);
}

FunctionButton::FunctionButton() : 
    FunctionButton(Number::UNKNOWN, "", "") 
{
}

FunctionButton::~FunctionButton() 
{
    //Serial.println("FunctionButton destructor called for button: " + this->key());
    for (int i = 0; i < _special_key_count; i++)
    {
        if (_special_key[i] != nullptr)
        {
            delete _special_key[i];
            _special_key[i] = nullptr;
        }
    }
}

void FunctionButton::set_key(const String &key)
{
    _key = key;
    for (int i = 0; i < _special_key_count; i++)
    {
        if (_special_key[i] != nullptr)
        {
            delete _special_key[i];
            _special_key[i] = nullptr;
        }
    }
    _special_key_count = _key_parser.parse_keys(_key, _special_key, MAX_SPECIAL_KEYS);
}

const FunctionButton::Number FunctionButton::number() const
{
    return _number;
}

const String &FunctionButton::key() const
{
    return _key;
}

FunctionButton::Number FunctionButton::get_number(unsigned int value)
{
    if (value < 0 || value > (MAX_BTN_NUMBER - 1))
    {
        return Number::UNKNOWN;
    }
    else 
    {
        return static_cast<Number>(value);
    }
}

String FunctionButton::get_number_string(FunctionButton::Number number)
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
      case Number::BTN_9:
        return "BTN_9";
      default:
        return "UNKNOWN";
    }
}

void FunctionButton::pressed()
{
    this->_action(  
        [](const String& s) { _keyboard_converter->print(s); },
        [](uint8_t k) { _keyboard_converter->press(k); 
    });
}

void FunctionButton::long_pressed()
{
    return;
}

void FunctionButton::released()
{
    this->_action(  
        nullptr,
        [](uint8_t k) { _keyboard_converter->release(k);
    });
}

void FunctionButton::_action(KeyboardStringAction write_function,
                             KeyboardIntAction special_function)
{
    //Serial.println("FunctionButton::_action key_count: " + String(_special_key_count));
    for (int i = 0; i < _special_key_count; i++)
    {
        if (_special_key[i] != nullptr)
        {
            //Serial.println("Special key found: " + _special_key[i]->toString());
            if (*_special_key[i] == SpecialKey::KeyType::TEXT)
            {
                Serial.print(F("Print "));
                Serial.println(_special_key[i]->value());
                if (write_function)
                {
                    (*write_function)(_special_key[i]->value());
                }
            }
            else
            {
                if (_keyboard_converter != nullptr)
                {
                    if (special_function)
                    {
                        uint8_t key_code = static_cast<uint8_t>(_keyboard_converter->operator()(*_special_key[i]));
                        Serial.print(F("Special "));
                        Serial.println(key_code);
                        (*special_function)(key_code);
                    }
                }
            }
        }
    }
}

Button::pin_number_t FunctionButton::_get_attached_pin(FunctionButton::Number number)
{
    switch (number)
    {
        case Number::BTN_1:
            return BTN_1_PIN;
        case Number::BTN_2:
            return BTN_2_PIN;
        case Number::BTN_3:
            return BTN_3_PIN;
        case Number::BTN_4:
            return BTN_4_PIN;
        case Number::BTN_5:
            return BTN_5_PIN;
        case Number::BTN_6:
            return BTN_6_PIN;
        case Number::BTN_7:
            return BTN_7_PIN;
        case Number::BTN_8:
            return BTN_8_PIN;
        case Number::BTN_9:
            return BTN_9_PIN;
        default:
            return 0; // Invalid pin
    }
}