/*
    led.h
    Evan Robinson, 2023-09-30

    Class to manage LED on arduino
    Presumes HIGH is on, LOW is off
*/

#ifndef led_h
#define led_h

#include <Arduino.h>

class LED {
    public:
        LED(uint8_t pin);
        void turnOn();
        void turnOff();
        bool isOn();
    private:
        uint8_t _pin;
        bool _isOn;
};

#endif
