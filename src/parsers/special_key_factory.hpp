#pragma once

#include <Arduino.h>
#include "special_key.hpp"

class SpecialKeyFactory
{
public:
  static SpecialKey* create_special_key(const String& key)
  {
    String k = key;
    k.toLowerCase();
    if (k == "ctrl")
    {
      return new SpecialKey(SpecialKey::KeyType::CTRL_LEFT);
    }
    else if (k == "shift")
    {
      return new SpecialKey(SpecialKey::KeyType::SHIFT);
    }
    else if (k == "tab")
    {
      return new SpecialKey(SpecialKey::KeyType::TAB);
    }
    else if (k == "f1")
    {
      return new SpecialKey(SpecialKey::KeyType::FUNCTION_1);
    }
    else if (k == "f2")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_2);
    }
    else if (k == "f3")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_3);
    }
    else if (k == "f4")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_4);
    }
    else if (k == "f5")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_5);
    }
    else if (k == "f6")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_6);
    }
    else if (k == "f7")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_7);
    }
    else if (k == "f8")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_8);
    }
    else if (k == "f9")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_9);
    }
    else if (k == "f10")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_10);
    }
    else if (k == "f11")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_11);
    }
    else if (k == "f12")
    {
        return new SpecialKey(SpecialKey::KeyType::FUNCTION_12);
    }
    else if (k == "alt")
    {
        return new SpecialKey(SpecialKey::KeyType::ALT_LEFT);
    }
    else if (k == "esc")
    {
        return new SpecialKey(SpecialKey::KeyType::ESCAPE);
    }
    else if (k == "enter")
    {
        return new SpecialKey(SpecialKey::KeyType::ENTER);
    }
    else if (k == "space")
    {
        return new SpecialKey(SpecialKey::KeyType::SPACE);
    }
    else if (k == "backspace")
    {
        return new SpecialKey(SpecialKey::KeyType::BACKSPACE);
    }
    else if (k == "delete")
    {
        return new SpecialKey(SpecialKey::KeyType::DELETE_KEY);
    }
    else if (k == "up")
    {
        return new SpecialKey(SpecialKey::KeyType::ARROW_UP);
    }
    else if (k == "down")
    {
        return new SpecialKey(SpecialKey::KeyType::ARROW_DOWN);
    }
    else if (k == "left")
    {
        return new SpecialKey(SpecialKey::KeyType::ARROW_LEFT);
    }
    else if (k == "right")
    {
        return new SpecialKey(SpecialKey::KeyType::ARROW_RIGHT);
    }
    else if (k == "win")
    {
        return new SpecialKey(SpecialKey::KeyType::WIN);
    }
    return new SpecialKey(SpecialKey::KeyType::TEXT, key);
  }
};
