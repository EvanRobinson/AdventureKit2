/*
    led.cpp
    Evan Robinson, 2023-09-30

    Class to manage LED on arduino
    Presumes HIGH is on, LOW is off
*/

#include "led.h"
#include <Arduino.h>

LED::LED(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    turnOff();
}

void LED::turnOn() {
    digitalWrite(_pin, HIGH);
    _isOn = true;
}

void LED::turnOff() {
    digitalWrite(_pin, LOW);
    _isOn = false;
}

bool LED::isOn() {
    return _isOn;
}
