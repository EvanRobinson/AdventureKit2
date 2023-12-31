/*
    photoresistor.cpp
    Evan Robinson, 2023-09-30

    Class to manage photoresistor on arduino
    Hand scaled -- recalibrate if you feel necessary
*/

#include "photoresistor.h"
#include <Arduino.h>

const int minPoint = 200;
const int maxPoint = 1000;

PhotoResistor::PhotoResistor(uint8_t pin) {
  _pin = pin;
  _value = 0.0;
}

double PhotoResistor::value(void) {
  int lightLevel = analogRead(_pin);
  lightLevel = map(lightLevel, minPoint, maxPoint, 0, 255); // Constrain to single byte
  lightLevel = constrain(lightLevel, 0, 255);
  _value = ((double)lightLevel) / 2.55; // scale to 0.0 to 100.0
  return _value;
}