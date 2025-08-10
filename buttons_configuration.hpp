#pragma once

#include "button.hpp"
#include <UnorderedMap.h>

using function_t = String;

struct ButtonArray {
  FunctionButton buttons[MAX_BTN_NUMBER];
};

class ButtonsConfiguration
{
public:
  using configuration_id_t = uint32_t;
  ButtonsConfiguration() : 
  _cfg_selected{0},
  _buttons_selected{nullptr} 
  {
  }

  configuration_id_t add_configuration(const function_t function_list[MAX_BTN_NUMBER])
  {
    configuration_id_t new_id = _get_new_id();

    ButtonArray *arr = new ButtonArray();
    for (int i = 0; i < MAX_BTN_NUMBER; i++) 
    {
      const FunctionButton::Number number = FunctionButton::get_number(i);
      arr->buttons[i] = {number, function_list[i]};
    }
    Serial.println("Add configuration done with ID: " + String(new_id));
    _config.put(static_cast<int>(new_id), arr);
    return new_id;
  }

  bool select_configuration(configuration_id_t id)
  {
    if (_cfg_selected == id)
    {
      return;
    }

    if (!_config.contains(static_cast<int>(id)))
    {
      Serial.println("Configuration id: " + String(id) + " not present!");
      return false;
    }
    _cfg_selected = id;
    _buttons_selected = this->get_configuration_btn(_cfg_selected);
    Serial.println("Configuration id: " + String(id) + " selected!");
    return true;
  }

  ButtonArray* get_selected_configuration_btn()
  {
    if (_buttons_selected == nullptr)
    {
      _buttons_selected = this->get_configuration_btn(_cfg_selected);
    }
    return _buttons_selected;
  }

  ButtonArray* get_configuration_btn(configuration_id_t id)
  {
    if (!_config.contains(static_cast<int>(id)))
    {
      return nullptr;
    }
    return _config.getValue(static_cast<int>(id));
  }

  void print_configuration()
  {
    Serial.print(F("CONFIGURATION: "));
    for (int i = 0; i < _config.getSize(); i++)
    {
      ButtonArray* btnArr = _config.getValue(static_cast<int>(i));

      Serial.print(F("Config ID "));
      Serial.print(i);
      Serial.println(F(":"));
      Serial.print(F("  Buttons: "));
      for (int b = 0; b < MAX_BTN_NUMBER; b++)
      {
        Serial.print(F("["));
        Serial.print(b);
        Serial.print(F("] "));
        Serial.print( FunctionButton::get_number_string(btnArr->buttons[b].number()));
        Serial.print(F("("));
        Serial.print(btnArr->buttons[b].key());
        Serial.print(F(")"));

        if (b < 7) Serial.print(F(", "));
      }
      Serial.println();
      Serial.println(F("---------------------------"));
    }
  }

private:
  configuration_id_t _get_new_id()
  {
    static configuration_id_t config_id = 0;
    return config_id++;
  }

  UnorderedMap<int, ButtonArray*> _config;
  ButtonArray* _buttons_selected;
  configuration_id_t _cfg_selected;
};