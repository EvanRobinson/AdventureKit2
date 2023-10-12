/*
    potentiometer.h
    Evan Robinson, 2023-10-11

    Class to manage Potentiometer
*/

#ifndef potentiometer
#define potentiometer

#include <Arduino.h>


class Potentiometer {
    public:
        Potentiometer(uint8_t pin);

        int read(void);
        double readScaledTo(double minValue, double maxValue);

    protected:
    private:
        uint8_t _pin;
        int _previousValue;
        int _currentValue;
};

#endif