#pragma once

#include <Keyboard.h>
#include "keyboard_key_converter.hpp"

class ArduinoKeyboardKeyConverter : public KeyboardKeyConverter
{
public:
    void press(uint8_t key) override
    {
        Keyboard.press(key);
    }

    void release(uint8_t key) override
    {
        Keyboard.release(key);
    }

    void print(const String& s) override
    {
        Keyboard.print(s);
    }

    int operator()(const SpecialKey &key) const override
    {
        switch (key)
        {
            case SpecialKey::KeyType::CTRL_LEFT:   return KEY_LEFT_CTRL;
            case SpecialKey::KeyType::CTRL_RIGHT:  return KEY_RIGHT_CTRL;
            case SpecialKey::KeyType::ALT:         return KEY_LEFT_ALT;
            case SpecialKey::KeyType::SHIFT:       return KEY_LEFT_SHIFT;
            case SpecialKey::KeyType::TAB:         return KEY_TAB;
            case SpecialKey::KeyType::ALT_LEFT:    return KEY_LEFT_ALT;
            case SpecialKey::KeyType::ESCAPE:      return KEY_ESC;
            case SpecialKey::KeyType::ENTER:       return KEY_RETURN;
            case SpecialKey::KeyType::SPACE:       return ' '; // per Arduino lo spazio è ASCII 32
            case SpecialKey::KeyType::BACKSPACE:   return KEY_BACKSPACE;
            case SpecialKey::KeyType::DELETE_KEY:  return KEY_DELETE;
            case SpecialKey::KeyType::ARROW_UP:    return KEY_UP_ARROW;
            case SpecialKey::KeyType::ARROW_DOWN:  return KEY_DOWN_ARROW;
            case SpecialKey::KeyType::ARROW_LEFT:  return KEY_LEFT_ARROW;
            case SpecialKey::KeyType::ARROW_RIGHT: return KEY_RIGHT_ARROW;
            case SpecialKey::KeyType::FUNCTION_1:  return KEY_F1;
            case SpecialKey::KeyType::FUNCTION_2:  return KEY_F2;
            case SpecialKey::KeyType::FUNCTION_3:  return KEY_F3;
            case SpecialKey::KeyType::FUNCTION_4:  return KEY_F4;
            case SpecialKey::KeyType::FUNCTION_5:  return KEY_F5;
            case SpecialKey::KeyType::FUNCTION_6:  return KEY_F6;
            case SpecialKey::KeyType::FUNCTION_7:  return KEY_F7;
            case SpecialKey::KeyType::FUNCTION_8:  return KEY_F8;
            case SpecialKey::KeyType::FUNCTION_9:  return KEY_F9;
            case SpecialKey::KeyType::FUNCTION_10: return KEY_F10;
            case SpecialKey::KeyType::FUNCTION_11: return KEY_F11;
            case SpecialKey::KeyType::FUNCTION_12: return KEY_F12;
            default: return -1; // chiave non gestita
        }
    }
};
