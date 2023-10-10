/*
    photoresistor.cpp
    Evan Robinson, 2023-09-30

    Class to manage photoresistor on arduino
    Hand scaled -- recalibrate if you feel necessary
*/

#include <Arduino.h>
#include "photoresistor.h"

const int minPoint = 200;
const int maxPoint = 1000;

PhotoResistor::PhotoResistor(uint8_t pin) {
    _pin = pin;
}

double PhotoResistor::value(void) {
    int lightLevel = analogRead(_pin);
    lightLevel = map(lightLevel, minPoint, maxPoint, 0, 255);
    lightLevel = constrain(lightLevel, 0, 255);
    _value = ((double) lightLevel) / 2.55;
    return _value;
}