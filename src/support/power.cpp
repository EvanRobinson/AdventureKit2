/*
    power.h
    Evan Robinson, 2023-10-03

    Power Management for our "dwelling"
    battery to store power
    solar charger to charge battery
*/

#include "power.h"
#include "LiquidCrystal_I2C.h"
#include "led.h"
#include "photoresistor.h"
#include "pins.h"
#include <Arduino.h>
#include <math.h>

// HouseBattery Storage
const double maximumBatteryPower = 100.0;
const double chargingThreshold = 90.0;
const double prettyFullThreshold = 80.0;
const double lowThreshold = 25.0;
const double criticalThreshold = 10.0;

HouseBattery::HouseBattery(void) {
  _battery = 0.0;
  _charging = false;
}

double HouseBattery::batteryLevel(void) {
  return _battery;
}

HouseBatteryPowerLevel HouseBattery::powerLevel(void) {
  if (_battery < criticalThreshold) {
    return PowerCritical;
  }
  else if (_battery < lowThreshold) {
    return PowerLow;
  }
  else if (_battery < prettyFullThreshold) {
    return PowerMiddle;
  }
  else if (_battery < chargingThreshold) {
    return PowerNearFull;
  }
  else {
    return PowerFull;
  }
}
bool HouseBattery::isCharging(void) {
  return _charging;
}

void HouseBattery::chargeBattery(double solarPower) {
  if (!_charging) {
    if (_battery < chargingThreshold) {
      _charging = true;
    }
  }

  double solar = solarPower / 50;
  if (_charging) {
    _battery += solar;
    _battery = min(_battery, maximumBatteryPower);
    if (_battery == maximumBatteryPower) {
      _charging = false;
    }
  }
}

void HouseBattery::usePower(double powerUsed) {
  _battery -= powerUsed;
  _battery = max(_battery, 0.0);
}
