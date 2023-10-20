/*
    dwelling.h
    Evan Robinson, 2023-10-20

    Dwelling class:
        absorb all the bits, in three categories:
            pieces of the house: alarmSystem, solarArray, etc.
            status displays: batteryStatusLight, etc.
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

#include <Arduino.h>
#include <Keypad.h>
#include "DigitalPinIO.h"
#include "led.h"
#include "LiquidCrystal_I2C.h"
#include "passive_buzzer.h"
#include "photoresistor.h"
#include "power.h"

class Dwelling {
    // components
    Buzzer alarmSystem;
    Power electricalStorage;
    DigitalPinOut exteriorFloodlights;
    DimmableLED interiorLights;
    DigitalPinIn intruderAlarm;
    PhotoResistor solarArray;   // from power


    // Status Displays
    RedGreenLED batteryStatusLight; // from power
    DigitalPinOut exteriorAlertLight;
    LiquidCrystal_I2C statusDisplay;

    // control board
    DigitalPinIn floodlightOverrideButton;
    DigitalPinIn interiorLightsButton;
    DigitalPinIn intruderAlarmOverrideButton;
    Keypad keypad;  // from main.cpp

    Dwelling(void); // all pins, etc. are hard coded in constructor
    void tick(void);    // run every 1/10 second
    void interiorLighting(void);
    void batteryChargingAndUsage(void);
    void securitySystems(int tickCount);
    void statusDisplays(void);
};
#endif