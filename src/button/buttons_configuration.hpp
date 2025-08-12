#pragma once

#include "function_button.hpp"
#include <EEPROM.h>

using function_t = String;

class ButtonArray {
public:
  FunctionButton* buttons[MAX_BTN_NUMBER];
  ~ButtonArray() { Serial.println(F("ButtonArray destroyed")); };
};

class ButtonsConfiguration
{
public:
  using index_t = uint32_t;
  ButtonsConfiguration();

  bool init();
  bool configuration_available();

  bool add_configuration(const function_t function_list[MAX_BTN_NUMBER]);
  bool remove_configuration(index_t index);
  bool select_configuration(index_t index);

  ButtonArray* get_selected_configuration_btn();
  ButtonArray* get_configuration_btn(index_t index);

  void print_configuration();
  const index_t current_index() const;

private:
  void _save_data();
  void _load_data();
  bool _add_configuration(const function_t function_list[MAX_BTN_NUMBER]);
  bool _select_configuration(index_t index);
  bool _remove_configuration(index_t index);

  ButtonArray* _config[MAX_CONFIGURATION_NUMBER];
  ButtonArray* _buttons_selected;

  uint8_t _config_size;
  index_t _cfg_selected;
};