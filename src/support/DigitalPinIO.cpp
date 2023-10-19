/*
    DigitalPinIn.cpp
    Evan Robinson, 2023-10-19

    Class to encapsulate simple digital input via pin
*/

#include <Arduino.h>
#include "DigitalPinIO.h"

// DigitalPinIn
DigitalPinIn::DigitalPinIn(uint8_t pin, bool pullup = withoutPullup, bool highIsOn = highOn) {
    _pin = pin;
    int mode = pullup ? INPUT_PULLUP : INPUT;
    pinMode(_pin, mode);

    _highIsOn = highIsOn;

    _lastReadValue = _highIsOn ? LOW : HIGH;
    _valueHasChanged = false;
}

bool DigitalPinIn::isOn(void) {
    bool on = _highIsOn ? value() == HIGH : value() == LOW;
    return on;
}

bool DigitalPinIn::isOff(void) {
    return !isOn();
}

bool DigitalPinIn::hasChanged(void) {
    return _valueHasChanged;
}

int DigitalPinIn::value(void) {
    // TBD debounce
    int currentValue = digitalRead(_pin);
    _valueHasChanged = (_lastReadValue != currentValue);
    _lastReadValue = currentValue;
    return currentValue;
}

// DigitalPinOut
DigitalPinOut::DigitalPinOut(uint8_t pin, bool highIsOn = highOn) {
    _pin = pin;
    pinMode(_pin, OUTPUT);

    _highIsOn = highIsOn;

    _lastSetValue = _highIsOn ? LOW : HIGH;
}

bool DigitalPinOut::isOn(void) {
    return _highIsOn ? _lastSetValue == HIGH : _lastSetValue == LOW;
}

bool DigitalPinOut::isOff(void) {
    return !isOn();
}

void DigitalPinOut::turnOn(void) {
    bool on = _highIsOn ? HIGH : LOW;
    _lastSetValue = on;
    digitalWrite(_pin, on);
}

void DigitalPinOut::turnOff(void) {
    bool off = _highIsOn ? LOW : HIGH;
    _lastSetValue = off;
    digitalWrite(_pin, off);
}

int DigitalPinOut::value(void) {
    return _lastSetValue;
}