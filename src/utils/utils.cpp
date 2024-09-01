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
    String c = input.substring(i, i+1);  // Extract one character as a String
    if (c == "á" || c == "à" || c == "ã" || c == "â") {
      output += 'a';
    } else if (c == "Á" || c == "À" || c == "Ã" || c == "Â") {
      output += 'A';
    } else if (c == "é" || c == "è" || c == "ê") {
      output += 'e';
    } else if (c == "É" || c == "È" || c == "Ê") {
      output += 'E';
    } else if (c == "í" || c == "ì" || c == "î") {
      output += 'i';
    } else if (c == "Í" || c == "Ì" || c == "Î") {
      output += 'I';
    } else if (c == "ó" || c == "ò" || c == "õ" || c == "ô") {
      output += 'o';
    } else if (c == "Ó" || c == "Ò" || c == "Õ" || c == "Ô") {
      output += 'O';
    } else if (c == "ú" || c == "ù" || c == "û") {
      output += 'u';
    } else if (c == "Ú" || c == "Ù" || c == "Û") {
      output += 'U';
    } else if (c == "ç") {
      output += 'c';
    } else if (c == "Ç") {
      output += 'C';
    } else {
      output += c;  // Add the character unchanged if no match is found
    }
  }
  return output;
}