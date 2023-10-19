/*
    DigitalPinIn.h
    Evan Robinson, 2023-10-19

    Class to encapsulate simple digital I/O via standard pin
*/

#include <Arduino.h>

#ifndef DigitalPinIn_h
#define DigitalPinIn_h

class DigitalPinIn {
public:
    DigitalPinIn(uint8_t pin, bool pullup, bool highOn);
    bool isOn(void);
    bool isOff(void);
    bool hasChanged(void);

protected:
    int value(void);
    int _lastReadValue;

private:
    uint8_t _pin;
    bool _highIsOn;

    bool _valueHasChanged;
};

class DigitalPinOut {
public:
    DigitalPinOut(uint8_t pin, bool highIsOn);
    bool isOn(void);
    bool isOff(void);
    void turnOn(void);
    void turnOff(void);
    bool hasChanged(void);
    void toggle(void);

protected:
    int value(void);
private:
    uint8_t _pin;
    bool _highIsOn;
    int _lastSetValue;
};

class DigitalPinIO {
public:
    static const bool withPullup = true;
    static const bool withoutPullup = false;
    static const bool highOn = true;
    static const bool lowOn = false;
};


#endif