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

DimmableLED::DimmableLED(uint8_t pwmPin) : LED(pwmPin) {
    _brightness = 128;  // middle of the road
}

void DimmableLED::dimmerLevel(uint8_t brightness) {
    _brightness = brightness;
    turnOn();
}

void DimmableLED::turnOn(void) {
    analogWrite(_pin, _brightness);
}

void DimmableLED::turnOff(void) {
    analogWrite(_pin, 0);
}

bool DimmableLED::isOn(void) {
    return (_brightness > 0);
}

uint8_t DimmableLED::brightness() {
    return _brightness;
}

RedGreenLED::RedGreenLED(uint8_t redPin, uint8_t greenPin) {
    _redPin = redPin;
    _greenPin = greenPin;
    _isOn = false;
    _isRed = false;
    _isGreen = false;
    _wasRed = false;
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    turnOff();
}

bool RedGreenLED::isRed(void) {
    return _isRed;
}

bool RedGreenLED::isGreen(void) {
    return _isGreen;
}

bool RedGreenLED::isOn(void) {
    return _isOn;
}

void RedGreenLED::turnOff(void) {
    digitalWrite(_redPin, LOW);
    digitalWrite(_greenPin, LOW);    
}

void RedGreenLED::turnOnRed(void) {
    digitalWrite(_redPin, HIGH);
    digitalWrite(_greenPin, LOW);
    _isOn = true;
    _isRed = true;
    _wasRed = true;
}

void RedGreenLED::turnOnGreen(void) {
    digitalWrite(_redPin, LOW);
    digitalWrite(_greenPin, HIGH);
    _isOn = true;
    _isGreen = true;
    _wasRed = false; 
}

bool RedGreenLED::wasRed(void) {
    return _wasRed;
}