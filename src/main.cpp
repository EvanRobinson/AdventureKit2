#include <Arduino.h>

const uint8_t whiteLEDControlPin = 22;
const uint8_t buttonInputPin = 24;
const int ledOnIndefintely = 0;
const long debounceInterval = 50L;
const int buttonPressed = 1;

// put function forward declarations here:
void turnLEDOnFor(int millis = 1000, uint8_t pin = whiteLEDControlPin);
void turnLEDOff(uint8_t pin = whiteLEDControlPin);
void toggleInteriorLights(void);

void setup() {
  pinMode(whiteLEDControlPin, OUTPUT);
  turnLEDOff();
  pinMode(buttonInputPin, INPUT);

  Serial.begin(9600);
  Serial.println("setup complete");
}


void loop() {
  static int previousPinValue = 0;
  static long previousDebounceTime = millis();
  int pinValue = digitalRead(buttonInputPin);

  if ((millis() - previousDebounceTime) < debounceInterval) {
    // debouncing: if the transition was less than debounceInterval ms ago, ignore it
    return;
  }

  if (previousPinValue != pinValue) {
    previousPinValue = pinValue;
    if (pinValue == buttonPressed) {
      toggleInteriorLights();
    }
  }
}

// put function definitions here:
void toggleInteriorLights(void) {
  static bool lightIsOn = false;

  if (lightIsOn) {
    turnLEDOff();
    lightIsOn = false;
  }
  else {
    turnLEDOnFor(ledOnIndefintely);
    lightIsOn = true;
  }
}

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