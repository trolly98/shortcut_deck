#pragma once

#include "serial_utils.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "button/button.hpp"

#define SWITCH_CFG_BTN_PIN 12

volatile int old_current_config = -2;
bool show_info = false;

class SwitchCfgButton : public Button
{
public:
  SwitchCfgButton() : Button(SWITCH_CFG_BTN_PIN) {}

protected:
  void pressed() override{}
  void long_pressed() override
  {
    show_info = true;
  }
  void released() override
  {
    if (show_info)
    {
      show_info = false;
      update_display();
      return;
    }

    global_buttons_configuration.select_configuration((global_buttons_configuration.current_index() + 1) % global_buttons_configuration.config_count());
  }
};
SwitchCfgButton switch_cfg_button;

void main_setup()
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("--------- SHORTCUT_DECK INIT ---------");
  pinMode(SWITCH_CFG_BTN_PIN, INPUT_PULLUP);

  global_display_manager.init();
  global_buttons_configuration.init();
  delay(100);
  update_display();
}

void main_loop() 
{
  switch_cfg_button.update();
  if (show_info)
  {
    show_display_info();
    return;
  }
  
  check_serial_config();

  if (global_buttons_configuration.configuration_available())
  {
    const ButtonArray* buttons_selected = global_buttons_configuration.get_selected_configuration_btn();
    if (buttons_selected)
    {
        for(int i = 0; i < MAX_BTN_NUMBER; i++)
      {
        if (buttons_selected->buttons[i] != nullptr)
        {
          buttons_selected->buttons[i]->update();
        }
      }
    }
  }

  if (old_current_config != global_buttons_configuration.current_index())
  {
    old_current_config = global_buttons_configuration.current_index();
    update_display();
  }


}