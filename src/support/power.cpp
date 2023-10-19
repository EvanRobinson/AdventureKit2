/*
    power.h
    Evan Robinson, 2023-10-03

    Power Management for our "dwelling"
    battery to store power
    solar charger to charge battery
*/

#include <Arduino.h>
#include <math.h>
#include "led.h"
#include "LiquidCrystal_I2C.h"
#include "photoresistor.h"
#include "pins.h"
#include "power.h"

// Power Storage
const double maximumBatteryPower = 100.0;
const double chargingThreshold = 90.0;
const double prettyFullThreshold = 80.0;
const double lowThreshold = 25.0;
const double criticalThreshold = 10.0;

const int blinkSpeedTicks = 5;


RedGreenLED batteryStatusLight = RedGreenLED(batteryLevelLEDRedPin, batteryLevelLEDGreenPin);


Power::Power(uint8_t photoResistorPin) :  _solarArray(photoResistorPin) {
    _pin = photoResistorPin;
    _battery = 0.0;
    _solar = 0.0;
    _charging = false;
}

double Power::batteryLevel(void) {
    return _battery;
}

double Power::solarPowerLevel(void) {
    return _solar;
}

bool Power::isCharging(void) {
    return _charging;
}

bool Power::isNearFull(void) {
    return _battery > chargingThreshold;
}

bool Power::isLow(void) {
    return _battery < lowThreshold;
}

bool Power::isCritical(void) {
    return _battery < criticalThreshold;
}

void Power::chargeBattery(void) {
    if (!_charging) {
        if (_battery < chargingThreshold) {
            _charging = true;
        } 
    }

    _solar = _solarArray.value() / 50;
    if (_charging) {
        _battery += _solar;
        _battery = min(_battery, maximumBatteryPower);
        if (_battery == maximumBatteryPower) {
            _charging = false;
        }
    }
}

void Power::tick(void) {
    static int tickCount = 0;

    tickCount++;

    bool blink = false;

    // blink red if low but not critical
    if (isLow() && !isCritical()) {
        blink = true;   // blinking red
    }
    if (_battery > prettyFullThreshold && !isNearFull()) {
        blink = true;   // blinking green
    }
    if (blink) {
        if ((tickCount % blinkSpeedTicks) == 0) {
            if (batteryStatusLight.isOn()) {
                batteryStatusLight.turnOff();
            }
            else {
                if (batteryStatusLight.wasRed()) {
                    batteryStatusLight.turnOnRed();
                }
                else {
                    batteryStatusLight.turnOnGreen();
                }
            }
        }
    }

    if (tickCount > blinkSpeedTicks) {
        tickCount = 0;
    }
}

void Power::usePower(double powerUsed) {
    _battery -= powerUsed;
    _battery = max(_battery, 0.0);
}

void Power::showStatus() {
    if (isNearFull()) {
        batteryStatusLight.turnOnGreen();
    }
    else if (_battery > prettyFullThreshold) {
        batteryStatusLight.turnOnGreen();
    }
    else if (isCritical()) {
        batteryStatusLight.turnOnRed();
    }
    else if (isLow()) {
        batteryStatusLight.turnOnRed();
    }
    else {
        batteryStatusLight.turnOff();
    }
}


void Power::showStatus(LiquidCrystal_I2C display) {
    display.setCursor(0,0);
    display.print("Batt:    ");
    display.setCursor(0,1);
    display.print("Solr:   ");

    display.setCursor(6, 0);
    display.print(int(_battery));
    display.setCursor(6, 1);
    display.print(int(_solarArray.value()));
}