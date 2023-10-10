/*
    photoresistor.h
    Evan Robinson, 2023-09-30

    Class to manage photoresistor on arduino
    Hand scaled -- recalibrate if you feel necessary
*/

#ifndef photoresistor_h
#define photoresistor_h

#include <Arduino.h>

class PhotoResistor {
    public:
        PhotoResistor(uint8_t pin);

        // provides a returned value from 0.0 to 100.0
        double value(void);
    protected:
    private:
        uint8_t _pin;
        double   _value;
};

#endif