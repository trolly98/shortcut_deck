#include "src/main.hpp"
#include "src/utils.hpp"

void setup() {
  main_setup();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // static unsigned long lastMillis = 0;
  // if (millis() - lastMillis >= 10000) {
  //   lastMillis = millis();
  //   //printFreeMemory();
  // }

  static unsigned long lastFlash = 0;
  if (millis() - lastFlash >= 300) {
    lastFlash = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  main_loop();
}