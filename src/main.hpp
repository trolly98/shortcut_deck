#include "serial_utils.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "button/button.hpp"

#define SWITCH_CFG_BTN_PIN 12

volatile int old_current_config = -2;

class SwitchCfgButton : public Button
{
public:
  SwitchCfgButton() : Button(SWITCH_CFG_BTN_PIN) {}

protected:
  void pressed() override{}
  void released() override
  {
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

  switch_cfg_button.update();

  if (old_current_config != global_buttons_configuration.current_index())
  {
    old_current_config = global_buttons_configuration.current_index();
    update_display();
  }
}