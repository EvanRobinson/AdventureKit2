/*
    dwelling.h
    Evan Robinson, 2023-10-20

    Dwelling class:
        absorb all the bits, in three categories:
            pieces of the house: _alarmSystem, _solarArray, etc.
            status displays: _batteryStatusLight, etc.
            control panel: keypad, buttons, etc.

        Constructor will connect all components to their hardware, in effect
            making the constructor where all the pin definitions, etc. are used
            This avoids passing a huge number of objects or other parms into the
            constructor while preserving the ability to move hardware around
            without having to change code in multiple places.

    As designed here (really just thoughts for now), the Dwelling is really the whole
        MVC enchilada: the components of the house (Model), the status displays (View),
        and the control board (Controller).
*/

#ifndef DWELLING_H
#define DWELLING_H

#include "DigitalPinIO.h"
#include "LiquidCrystal_I2C.h"
#include "led.h"
#include "passive_buzzer.h"
#include "photoresistor.h"
#include "power.h"
#include <Arduino.h>
#include <Keypad.h>

class Dwelling {
public:
  void init(void);

  // components
  Buzzer _alarmSystem;
  HouseBattery _electricalStorage;
  DigitalPinOut _exteriorLights;
  DimmableLED _interiorLights;
  DigitalPinIn _intruderAlarm;
  PhotoResistor _solarArray; // from power

  // Status Displays
  RedGreenLED _batteryStatusLight; // from power
  DigitalPinOut _exteriorAlertLight;
  LiquidCrystal_I2C _statusDisplay;

  // control board
  DigitalPinIn _exteriorLightsButton;
  DigitalPinIn _interiorLightsButton;
  Keypad _keypad; // from main.cpp

  Dwelling(void);           // all pins, etc. are hard coded in constructor
  void tick(int tickCount); // run every 1/10 second
  void lighting(void);
  void batteryChargingAndUsage(void);
  void securitySystems(int tickCount);
  void statusDisplays(int tickCount);

private:
  void printToStatusDisplay(uint8_t x, uint8_t y, uint8_t valueOffset, const char *clearString, int value);
  bool printIndicatorToStatusDisplay(uint8_t x, uint8_t y, bool print, const char indicator);
  void houseBatteryStatusLight(int tickCount);

  bool _exteriorLightsTurnedOnManually;
};
#endif