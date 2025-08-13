#include "utils.hpp"

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