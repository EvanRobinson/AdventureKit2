/*
    potentiometer.cpp
    Evan Robinson, 2023-10-11

    Class to manage Potentiometer
*/

#include <Arduino.h>
#include <math.h>
#include "potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin) {
    _pin = pin;
    _scale = 1.0;
    _previousValue = 0;
    _currentValue = 0;
}

int Potentiometer::read(void) {
    _currentValue = analogRead(_pin);
    return _currentValue;
}

double Potentiometer::readScaledTo(double minValue, double maxValue) {
    read();
    double rangeSize = maxValue - minValue;
    double readingSize = 1023.0;

    double scaled = minValue + (_currentValue * rangeSize / readingSize);
    scaled = min(scaled, maxValue);
    scaled = max(minValue, scaled);
    return scaled;
}
