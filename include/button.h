/*
    button.h
    Evan Robinson, 2023-09-30

    Class to manage momentary button on arduino
    Button presumes a pulldown resistor on the wired button (i.e. that pressed is HIGH)
    ButtonPullUp presumes an internal pullup resistor (that pressed is LOW).  Only change needed
        to manage an external pullup resistor is to change the ButtonPullUp constructor to not call
        pinMode();
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
        uint8_t _pin;
        int _value;

    private:
        int _valueHasChanged;
};

class ButtonPullUp : public Button {
    public:
        ButtonPullUp(uint8_t pin);
        bool isPressed();
};

#endif
