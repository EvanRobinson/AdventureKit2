/*
    passive_buzzer.cpp
    Evan Robinson, 2023-10-10

    Class to manage Passive Buzzer
*/

#include <Arduino.h>
#include "passive_buzzer.h"

int tickCount = 0;
int alarmDuration = 10; // 10 ticks == 1 second

Buzzer::Buzzer(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    _alarm = noAlarm;
    _previousAlarm = noAlarm;
    _alarmTime = tickCount;
}

void Buzzer::alarm(AlarmSignals signal) {
    static int signalFreq[] = {0, 440, 880, 220}; // TBD: Fix magic number
    static int signalDuration = 1000; // TBD: Fix magic number

    // don't repeat the same alarm
    if (signal != _alarm) {
        switch (signal) {
            case noAlarm:
                alarmOff();
                break;
            case alert:
            case power_critical:
            case power_low:
                _alarm = signal;
                if (_alarm != _previousAlarm) {
                    _previousAlarm = _alarm;
                    _alarmTime = tickCount;

                    play(signalFreq[signal], signalDuration);
                }
                break;
            default:
                Serial.print("unknown alarm signal: ");
                Serial.println(signal);
                alarmOff();
                break;
        }
    }
}

void Buzzer::alarmOff() {
    noTone(_pin);
    _previousAlarm = _alarm;
    _alarm = noAlarm;
    _alarmTime = tickCount;
}

void Buzzer::tick() {

    // manage changing ongoing sound every 1/10 second
    tickCount++;

    if ((tickCount - _alarmTime) > alarmDuration) {
        alarmOff();
    }
}

void Buzzer::play(int frequency, unsigned long duration) {
    tone(_pin, frequency, duration);
}
