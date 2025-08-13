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

// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop() {
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(500);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(500);
// }