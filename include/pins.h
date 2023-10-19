/*
    pins.h
    Evan Robinson, 2023-10-18

    Single location for all hardware pin assignments
*/

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
const uint8_t exteriorFloodlightsPin = 32;
const uint8_t exteriorAlertLightPin = 33;
