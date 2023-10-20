/*
    pins.h
    Evan Robinson, 2023-10-18

    Single location for all hardware pin assignments
*/

#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// Analog Pins
const uint8_t solarArrayAnalogInputPin = 0;

// PWM Pins
const uint8_t interiorLightsPWMControlPin = 12;
const uint8_t alarmSystemPWMPin = 13;

// Digital Pins
const uint8_t interiorLightsButtonPin = 24;
const uint8_t batteryLevelLEDRedPin = 26;
const uint8_t batteryLevelLEDGreenPin = 27;
const uint8_t intruderMotionAlarmPin = 28;
const uint8_t floodlightOverridePin = 29;
const uint8_t intruderAlarmOverridePin = 31;
const uint8_t exteriorAlertLightPin = 32;
const uint8_t exteriorFloodlightsPin = 33;
// Keypad
const uint8_t keypad00 = 34;
const uint8_t keypad01 = 35;
const uint8_t keypad02 = 36;
const uint8_t keypad03 = 37;
const uint8_t keypad04 = 38;
const uint8_t keypad05 = 39;
const uint8_t keypad06 = 40;
const uint8_t keypad07 = 41;

#endif