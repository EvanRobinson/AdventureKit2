/*
    power.h
    Evan Robinson, 2023-10-03

    Power Management for our "dwelling"
    battery to store power
    solar charger to charge battery
*/

#ifndef power_h
#define power_h

#include <Arduino.h>
#include "photoresistor.h"
#include "LiquidCrystal_I2C.h"

class Power {
    public:
        Power(uint8_t photoResistorPin);
        void usePower(double powerUsed);
        void chargeBattery();

        // provides a returned value from 0.0 to 100.0
        double batteryLevel(void);
        double solarPowerLevel(void);
        bool isCharging(void);
        bool isNearFull(void);
        bool isLow(void);
        bool isCritical(void);

        void showStatus(void);          // manage indicator lights and alarms
        void showStatus(LiquidCrystal_I2C display); // manage always on display
        void showIndicatorLights(bool full, bool critical, bool low, bool audio);

        virtual void tick(void);
    protected:
    private:
        PhotoResistor _solarArray;
        double _battery;
        double _solar;
        uint8_t _pin;
        bool _charging;
};

#endif