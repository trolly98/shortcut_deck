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

void update_display()
{
  global_display_manager.display()->print_configuration(global_buttons_configuration.get_selected_configuration_btn(), 
                                             global_buttons_configuration.current_index() + 1,
                                             global_buttons_configuration.config_count());
}