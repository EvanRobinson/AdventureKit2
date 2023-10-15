/*
    button.cpp
    Evan Robinson, 2023-09-30

    Class to manage momentary button on arduino
    Presumes HIGH is pressed, LOW is not pressed
    Press occurs on down press of button
*/

#include "button.h"
#include <Arduino.h>

Button::Button(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, INPUT);
    _value = value();
    _valueHasChanged = false;
}

bool Button::isPressed() {
    _value = value();
    return (_value == HIGH);
}

int Button::value() {
    // TBD: Debounce
    int currentValue = digitalRead(_pin);
    _valueHasChanged = (_value != currentValue);
    return digitalRead(_pin);
}

bool Button::hasChanged() {
    return _valueHasChanged;
}


ButtonPullUp::ButtonPullUp(uint8_t pin) : Button(pin) {
    pinMode(_pin, INPUT_PULLUP);
    _value = value();
}

bool ButtonPullUp::isPressed() {
    _value = value();
    return (_value == LOW);
}