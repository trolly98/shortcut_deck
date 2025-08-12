#include "utils.hpp"

#include <Arduino.h>

extern int __heap_start, *__brkval;

void printFreeMemory() {
  int v;
  int val = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);

  Serial.print("Free RAM: ");
  Serial.println(val);
}