#include <Arduino.h>
#include "utils.h"

void blink(int times, int delayMs, int LED) {
  for (int i = 0; i < times; i++) {
    delay(delayMs);
    digitalWrite(LED, LOW);
    delay(delayMs);
    digitalWrite(LED, HIGH);
  }
}
