/*
    passive_buzzer.h
    Evan Robinson, 2023-10-10

    Class to manage Passive Buzzer
*/

#ifndef passive_buzzer_h
#define passive_buzzer_h

#include <Arduino.h>

typedef enum {
  noAlarm = 0,
  alert,
  power_critical,
  power_low
} AlarmSignals;

class Buzzer {
public:
  Buzzer(uint8_t pin);
  void alarm(AlarmSignals signal);
  void alarmOff(void);

  void play(int frequency, unsigned long duration);

protected:
private:
  uint8_t _pin;
  AlarmSignals _alarm;
  AlarmSignals _previousAlarm;
};

#endif