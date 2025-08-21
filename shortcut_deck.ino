#include "src/main.hpp"
#include "src/utils.hpp"

void setup() {
  main_setup();
}

void loop() {
  static unsigned long lastMillis = 0;
  if (millis() - lastMillis >= 10000) {
    lastMillis = millis();
    printFreeMemory();
  }
  main_loop();
}