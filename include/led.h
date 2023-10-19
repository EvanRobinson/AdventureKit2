/*
    led.h
    Evan Robinson, 2023-09-30

    Classes to manage single color LED,
        Red/Green Bicolor LED
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
    protected:
        uint8_t _pin;
        bool _isOn;
    private:

};

class DimmableLED : LED {
    public:
        DimmableLED(uint8_t pwmPin);
        void dimmerLevel(uint8_t brightness);
        void turnOn();
        void turnOff();
        bool isOn();
    private:
        uint8_t _brightness;
};


class RedGreenLED {
    public:
        RedGreenLED(uint8_t batteryLevelLEDRedPin, uint8_t batteryLevelLEDGreenPin);
        bool isOn(void);
        bool isRed(void);
        bool isGreen(void);
        void turnOff(void);
        void turnOnRed(void);
        void turnOnGreen(void);
        bool wasRed(void);

    private:
        uint8_t _redPin;
        uint8_t _greenPin;
        bool _isOn;
        bool _isRed;
        bool _isGreen;
        bool _wasRed;
};

#endif