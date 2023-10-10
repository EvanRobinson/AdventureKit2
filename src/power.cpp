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
#include "photoresistor.h"
#include "power.h"

// Power Storage
const double maximumBatteryPower = 100.0;
const double chargingThreshold = 90.0;
const double prettyFullThreshold = 80.0;
const double lowThreshold = 25.0;
const double criticalThreshold = 10.0;

const int blinkSpeedTicks = 5;

const int redPin = 26;
const int greenPin = 27;
RedGreenLED batteryStatusLight = RedGreenLED(redPin, greenPin);


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

void Power::showStatus(void) {
    bool audioAlarm = false;

    // TBD: Output _battery and _solar levels to display
    if (isNearFull()) {
        batteryStatusLight.turnOnGreen();
    }
    else if (_battery > prettyFullThreshold) {
        batteryStatusLight.turnOnGreen();
    }
    else if (isCritical()) {
        batteryStatusLight.turnOnRed();
        // TBD: repeating audio warning beep
        audioAlarm = true;
    }
    else if (isLow()) {
        batteryStatusLight.turnOnRed();
        // TBD: one-time audio warning beep
        audioAlarm = true;
    }
    else {
        batteryStatusLight.turnOff();
    }
    // TBD: Remove the test when this info is sent to hardware display instead of Serial
    if (_battery != maximumBatteryPower) {
        char buffer[50];
        snprintf(buffer, 50, "Battery: %d%% Solar:%d", (int) _battery, (int) _solarArray.value());
        Serial.println(buffer);
    }
}