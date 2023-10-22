/*
    power.h
    Evan Robinson, 2023-10-03

    Power Management for our "dwelling"
    battery to store power
    solar charger to charge battery
*/

#ifndef power_h
#define power_h

#include "LiquidCrystal_I2C.h"
#include "photoresistor.h"
#include <Arduino.h>

typedef enum {
  PowerCritical = 0,
  PowerLow,
  PowerMiddle,
  PowerNearFull,
  PowerFull
} HouseBatteryPowerLevel;

class HouseBattery {
public:
  HouseBattery(void);
  void usePower(double powerUsed);
  void chargeBattery(double solarPower);

  // provides a returned value from 0.0 to 100.0
  double batteryLevel(void);
  HouseBatteryPowerLevel powerLevel(void);

  bool isCharging(void);

protected:
private:
  double _battery;
  bool _charging;
};

#endif