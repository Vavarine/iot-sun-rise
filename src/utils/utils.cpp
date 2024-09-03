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

String removeAccents(String input) {
  String output = "";

  for (unsigned i = 0; i < input.length(); i++) {
    char c = input[i];

    if ((c & 0xE0) == 0xC0) {  // Two-byte character
      i++;  // Move to the second byte
      char next_c = input[i];
      if ((c == 0xC3) && (next_c == 0xA1 || next_c == 0xA0 || next_c == 0xA3 || next_c == 0xA2)) {
        output += 'a';
      } else if ((c == 0xC3) && (next_c == 0x81 || next_c == 0x80 || next_c == 0x83 || next_c == 0x82)) {
        output += 'A';
      } else if ((c == 0xC3) && (next_c == 0xA9 || next_c == 0xA8 || next_c == 0xAA)) {
        output += 'e';
      } else if ((c == 0xC3) && (next_c == 0x89 || next_c == 0x88 || next_c == 0x8A)) {
        output += 'E';
      } else if ((c == 0xC3) && (next_c == 0xAD || next_c == 0xAC || next_c == 0xAE)) {
        output += 'i';
      } else if ((c == 0xC3) && (next_c == 0x8D || next_c == 0x8C || next_c == 0x8E)) {
        output += 'I';
      } else if ((c == 0xC3) && (next_c == 0xB3 || next_c == 0xB2 || next_c == 0xB5 || next_c == 0xB4)) {
        output += 'o';
      } else if ((c == 0xC3) && (next_c == 0x93 || next_c == 0x92 || next_c == 0x95 || next_c == 0x94)) {
        output += 'O';
      } else if ((c == 0xC3) && (next_c == 0xBA || next_c == 0xB9 || next_c == 0xBB)) {
        output += 'u';
      } else if ((c == 0xC3) && (next_c == 0x9A || next_c == 0x99 || next_c == 0x9B)) {
        output += 'U';
      } else if (c == 0xC3 && next_c == 0xA7) {
        output += 'c';
      } else if (c == 0xC3 && next_c == 0x87) {
        output += 'C';
      } else {
        output += c;
        output += next_c;
      }
    } else {
      output += c;
    }
  }

  return output;
}

