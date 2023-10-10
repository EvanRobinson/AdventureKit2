#include <Arduino.h>

const uint8_t whiteLEDControlPin = 22;
const int ledOnIndefintely = 0;

// put function forward declarations here:
void turnLEDOnFor(int millis = 1000, uint8_t pin = whiteLEDControlPin);
void turnLEDOff(uint8_t pin = whiteLEDControlPin);

void setup() {
  pinMode(whiteLEDControlPin, OUTPUT);
}

#define THROW_AWAY_CODE

void loop() {
#ifdef THROW_AWAY_CODE
  digitalWrite(whiteLEDControlPin, HIGH);
  delay(1000);
  digitalWrite(whiteLEDControlPin, LOW);
  delay(1000);
#else
  turnLEDOnFor(300);
  delay(300);
#endif
}

// put function definitions here:
void turnLEDOnFor(int millis, uint8_t pin) {
  digitalWrite(pin, HIGH);
  if (millis != ledOnIndefintely) {
    delay(millis);
    turnLEDOff();
  }
}

void turnLEDOff(uint8_t pin) {
  digitalWrite(pin, LOW);
}