#include "utils.hpp"
#include "globals.hpp"
#include <Arduino.h>

#if defined(__AVR__)
extern int __heap_start, *__brkval;
void printFreeMemory() {
  int v;
  int val = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);

  Serial.print("Free RAM: ");
  Serial.println(val);
}
#else
extern "C" char* _sbrk(int incr);

int freeMemory() {
  char top;
  return &top - _sbrk(0);
}
void printFreeMemory() {
  Serial.print("Free RAM: ");
  Serial.println(freeMemory());
}
#endif


volatile int old_current_config = -2;
bool is_display_need_to_be_updated()
{
  return (old_current_config != global_buttons_configuration.current_index());
}

void update_display()
{
  old_current_config = global_buttons_configuration.current_index();
  global_display_manager.display()->print_configuration(global_buttons_configuration.get_selected_configuration_btn(), 
                                             global_buttons_configuration.current_index() + 1,
                                             global_buttons_configuration.config_count());
}

void show_display_info()
{
  global_display_manager.display()->show_info();
}