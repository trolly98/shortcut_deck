#pragma once

class SpecialKey
{
public:
  enum class KeyType
  {
    TEXT,
    CTRL_LEFT,
    CTRL_RIGHT,
    ALT,
    SHIFT,
    TAB,
    ALT_LEFT,
    ESCAPE,
    ENTER,
    SPACE,
    BACKSPACE,
    DELETE_KEY,
    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    FUNCTION_1,
    FUNCTION_2,
    FUNCTION_3,
    FUNCTION_4,
    FUNCTION_5,
    FUNCTION_6,
    FUNCTION_7,
    FUNCTION_8,
    FUNCTION_9,
    FUNCTION_10,
    FUNCTION_11,
    FUNCTION_12
  };

  SpecialKey(KeyType key, const String &value = "") : _key(key), _value(value) {};
  ~SpecialKey() {};
  operator KeyType() const { return _key; }
  const String &value() const { return _value; }
  String toString() const
  {
    switch (_key)
    {
      case KeyType::TEXT: return "TEXT";
      case KeyType::CTRL_LEFT: return "CTRL_LEFT";
      case KeyType::CTRL_RIGHT: return "CTRL_RIGHT";
      case KeyType::ALT: return "ALT";
      case KeyType::SHIFT: return "SHIFT";
      case KeyType::TAB: return "TAB";
      case KeyType::ALT_LEFT: return "ALT_LEFT";
      case KeyType::ESCAPE: return "ESCAPE";
      case KeyType::ENTER: return "ENTER";
      case KeyType::SPACE: return "SPACE";
      case KeyType::BACKSPACE: return "BACKSPACE";
      case KeyType::DELETE_KEY: return "DELETE_KEY";
      case KeyType::ARROW_UP: return "ARROW_UP";
      case KeyType::ARROW_DOWN: return "ARROW_DOWN";
      case KeyType::ARROW_LEFT: return "ARROW_LEFT";
      case KeyType::ARROW_RIGHT: return "ARROW_RIGHT";
      case KeyType::FUNCTION_1: return "FUNCTION_1";
      case KeyType::FUNCTION_2: return "FUNCTION_2";
      case KeyType::FUNCTION_3: return "FUNCTION_3";
      case KeyType::FUNCTION_4: return "FUNCTION_4";
      case KeyType::FUNCTION_5: return "FUNCTION_5";
      case KeyType::FUNCTION_6: return "FUNCTION_6";
      case KeyType::FUNCTION_7: return "FUNCTION_7";
      case KeyType::FUNCTION_8: return "FUNCTION_8";
      case KeyType::FUNCTION_9: return "FUNCTION_9";
      case KeyType::FUNCTION_10: return "FUNCTION_10";
      case KeyType::FUNCTION_11: return "FUNCTION_11";
      case KeyType::FUNCTION_12: return "FUNCTION_12";
    }
    return "";
  }

private:
  KeyType _key;
  String _value;
};
