/*
    DigitalPinIn.h
    Evan Robinson, 2023-10-19

    Class to encapsulate simple digital I/O via standard pin
*/

#include <Arduino.h>

class DigitalPinIn {
public:
    static const bool withPullup = true;
    static const bool withoutPullup = false;
    static const bool highOn = true;
    static const bool lowOn = false;

    DigitalPinIn(uint8_t pin, bool pullup, bool highOn);
    bool isOn(void);
    bool isOff(void);
    bool hasChanged(void);

protected:
    int value(void);
private:
    uint8_t _pin;
    bool _highIsOn;
    int _lastReadValue;
    bool _valueHasChanged;
};

class DigitalPinOut {
public:
    static const bool highOn = true;
    static const bool lowOn = false;

    DigitalPinOut(uint8_t pin, bool highIsOn);
    bool isOn(void);
    bool isOff(void);
    void turnOn(void);
    void turnOff(void);
    bool hasChanged(void);

protected:
    int value(void);
private:
    uint8_t _pin;
    bool _highIsOn;
    int _lastSetValue;
};