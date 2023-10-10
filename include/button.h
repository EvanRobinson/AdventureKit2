/*
    button.h
    Evan Robinson, 2023-09-30

    Class to manage momentary button on arduino
    Presumes HIGH is pressed, LOW is not pressed
    Press occurs on down press of button
*/

#ifndef button_h
#define button_h

#include <Arduino.h>

class Button {
    public:
        Button(uint8_t pin);
        bool isPressed();
        bool hasChanged();
        int value();
    protected:

    private:
        uint8_t _pin;
        int _value;
        int _valueHasChanged;
};

#endif
