#include "serial_utils.hpp"
#include "globals.hpp"

void setup()
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("--------- SHORTCUT_DECK INIT ---------");

  global_buttons_configuration.init();
}

void loop() 
{
  check_serial_config();

  if (!global_buttons_configuration.configuration_available())
  {
    return;
  }
  const ButtonArray* buttons_selected = global_buttons_configuration.get_selected_configuration_btn();
  if (!buttons_selected)
  {
    return;
  }

  for(int i = 0; i < MAX_BTN_NUMBER; i++)
  {
    buttons_selected->buttons[i].update();
  }
}
