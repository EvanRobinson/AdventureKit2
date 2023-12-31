/*
    dwelling.cpp
    Evan Robinson, 2023-10-20

    Design notes are in the .h file
*/

#include "dwelling.h"

#include <Arduino.h>
#include <Keypad.h>

#include "DigitalPinIO.h"
#include "LiquidCrystal_I2C.h"
#include "pins.h"

const int interiorLightsPowerUsage = 1;
const int exteriorLightsPowerUsage = 3;

// interiorLights levels:
const int interiorLightsCritical = 2;
const int interiorLightsLow = 63;
const int interiorLightsNormal = 255;

// _keypad
const byte keypadRows = 4;
const byte keypadColumns = 4;

const char keys[keypadRows][keypadColumns] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
};
byte rowPins[keypadRows] = {keypad00, keypad01, keypad02, keypad03};
byte columnPins[keypadColumns] = {keypad04, keypad05, keypad06, keypad07};

Dwelling::Dwelling(void) :
    _alarmSystem(alarmSystemPWMPin),
    _electricalStorage(),
    _exteriorLights(exteriorFloodlightsPin, DigitalPinIO::highOn),
    _interiorLights(interiorLightsPWMControlPin),
    _intruderAlarm(intruderMotionAlarmPin, DigitalPinIO::withoutPullup, DigitalPinIO::highOn),
    _solarArray(solarArrayAnalogInputPin),
    _batteryStatusLight(batteryLevelLEDRedPin, batteryLevelLEDGreenPin),
    _exteriorAlertLight(exteriorAlertLightPin, DigitalPinIO::highOn),
    _statusDisplay(0x27, 16, 2),
    _exteriorLightsButton(exteriorLightsButtonPin, DigitalPinIO::withPullup, DigitalPinIO::lowOn),
    _interiorLightsButton(interiorLightsButtonPin, DigitalPinIO::withPullup, DigitalPinIO::lowOn),
    _accessStatus(lockRedPin, lockGreenPin),
    _keypad(makeKeymap(keys), rowPins, columnPins, keypadRows, keypadColumns) {
  _exteriorLightsTurnedOnManually = false;
  _unlocked = false;
}

void Dwelling::init(void) {
  _accessStatus.turnOnRed();
  if (_unlocked) {
    _accessStatus.turnOnGreen();
  }
  initStatusDisplay();
}

void Dwelling::tick(int tickCount) {
  const int ticksPerLighting = 1;     // lighting input happens every tick
  const int ticksPerMotionSensor = 1; // motion sensor checked every tick
  const int ticksPerCharging = 10;    // charging happens every second

  statusDisplays(tickCount);
  houseBatteryStatusLight(tickCount);

  if ((tickCount % ticksPerLighting) == 0) {
    lighting();
  }

  if ((tickCount % ticksPerCharging) == 0) {
    batteryChargingAndUsage();
  }

  if ((tickCount % ticksPerMotionSensor) == 0) {
    exteriorMotionDetector(tickCount);
  }
}

void Dwelling::unlock(void) {
  const int codeLength = 6;
  const int failureLimit = 3;
  static int failures = 0;

  initStatusDisplay(); // clear status display, make sure it's backlit
  while (!_unlocked) {
    _statusDisplay.setCursor(0, 0);
    _statusDisplay.print("Input PIN:     ");
    const char code[codeLength + 1] = {'7', '4', '5', '2', 'A', '0', 0}; // + 1 accounts for terminal 0
    char input[codeLength + 1] = {' ', ' ', ' ', ' ', ' ', ' ', 0};      // + 1 accounts for terminal 0

    for (int inputChars = 0; inputChars < codeLength; inputChars++) {
      char key = _keypad.waitForKey();
      if (key) {
        input[inputChars] = key;
        _statusDisplay.setCursor(10 + inputChars, 0);
        _statusDisplay.print("*");
      }
    }
    bool goodCode = true;
    for (int codeIndex = 0; codeIndex < codeLength; codeIndex++) {
      if (input[codeIndex] != code[codeIndex]) {
        goodCode = false;
        break;
      }
    }
    if (goodCode) {
      _unlocked = true;
      _accessStatus.turnOff();
      _accessStatus.turnOnGreen();
      Serial.println("Unlocked");
      _statusDisplay.clear();
      printToStatusDisplay(0, 0, "System Unlocked");
      delay(2000);
      _statusDisplay.clear();
    }
    else {
      failures++;
      if (failures == failureLimit) {
        _statusDisplay.clear();
        printToStatusDisplay(0, 0, "There Will Be A");
        printToStatusDisplay(0, 1, "15 Second Delay");
        delay(15000);
        _statusDisplay.clear();
        failures = 0;
      }
      else {
        _statusDisplay.clear();
        printToStatusDisplay(0, 0, 10, "Bad Code:", failures);
        printToStatusDisplay(0, 1, "Try Again");
        delay(5000);
      }

      _statusDisplay.clear();
    }
  }
}

void Dwelling::initStatusDisplay(void) {
  _statusDisplay.init();
  _statusDisplay.clear();
  _statusDisplay.backlight();
}

void Dwelling::lighting() {
  // Turn _interiorLights on and off using button
  static bool interiorLightsButtonWasPressed = false;
  static bool exteriorLightsButtonWasPressed = false;
  if (_interiorLightsButton.isOn()) {
    if (!interiorLightsButtonWasPressed) {
      // button has gone from unpressed to pressed
      if (_interiorLights.isOn()) {
        _interiorLights.turnOff();
      }
      else {
        _interiorLights.turnOn();
      }
    }
    interiorLightsButtonWasPressed = true;
  }
  else {
    interiorLightsButtonWasPressed = false;
  }
  if (_exteriorLightsButton.isOn()) {
    if (!exteriorLightsButtonWasPressed) {
      // button has gone from unpressed to pressed
      if (_exteriorLights.isOn()) {
        _exteriorLights.turnOff();
      }
      else {
        _exteriorLights.turnOn();
        _exteriorLightsTurnedOnManually = true;
      }
    }
    exteriorLightsButtonWasPressed = true;
  }
  else {
    exteriorLightsButtonWasPressed = false;
  }

  // manage interior light brightness based upon available power
  if (_interiorLights.isOn()) {
  }
  // manage lights depending upon current power levels
  if (_interiorLights.isOn() || _exteriorLights.isOn()) {
    if (_electricalStorage.powerLevel() == PowerCritical) {
      _alarmSystem.alarm(power_critical);
      if (_interiorLights.isOn()) {
        _interiorLights.dimmerLevel(interiorLightsCritical); //
        _interiorLights.turnOn();
      }
      if (_exteriorLights.isOn()) {
        _exteriorLights.turnOff();
      }
    }
    else if (_electricalStorage.powerLevel() == PowerLow) {
      _alarmSystem.alarm(power_low);
      if (_interiorLights.isOn()) {
        _interiorLights.dimmerLevel(interiorLightsLow);
        _interiorLights.turnOn();
      }
    }
  }
  else {
    if (_interiorLights.isOn()) {
      _interiorLights.dimmerLevel(interiorLightsNormal);
      _interiorLights.turnOn();
    }
  }
}

void Dwelling::batteryChargingAndUsage() {
  static double solarPower = _solarArray.value();

  _electricalStorage.chargeBattery(solarPower);

  // account for interiorLights power usage
  if (_interiorLights.isOn()) {
    _electricalStorage.usePower(interiorLightsPowerUsage);
  }
  if (_exteriorLights.isOn()) {
    _electricalStorage.usePower(exteriorLightsPowerUsage);
  }
}

/* Status Display Plan
0000000000111111
0123456789012345
T XXXX ieA LFA
B XXX S XXX

i = interior lights on
e = exterior floodlights on
A = intruder alarm active
L = interior light switch pressed
F = floodlight switch pressed
*/
void Dwelling::statusDisplays(int tickCount) {
  printToStatusDisplay(0, 0, 2, "T     ", tickCount / 10);
  printToStatusDisplay(0, 1, 2, "B    ", int(_electricalStorage.batteryLevel()));
  printToStatusDisplay(6, 1, 2, "S    ", int(_solarArray.value()));

  printIndicatorToStatusDisplay(7, 0, _interiorLights.isOn(), 'i');
  printIndicatorToStatusDisplay(8, 0, _exteriorLights.isOn(), 'e');
  printIndicatorToStatusDisplay(9, 0, _intruderAlarm.isOn(), 'A');

  printIndicatorToStatusDisplay(11, 0, _interiorLightsButton.isOn(), 'L');
  printIndicatorToStatusDisplay(12, 0, _exteriorLightsButton.isOn(), 'E');
}

void Dwelling::printToStatusDisplay(uint8_t x, uint8_t y, const char *string) {
  _statusDisplay.setCursor(x, y);
  _statusDisplay.print(string);
}

void Dwelling::printToStatusDisplay(uint8_t x, uint8_t y, int value) {
  _statusDisplay.setCursor(x, y);
  _statusDisplay.print(value);
}

void Dwelling::printToStatusDisplay(uint8_t x, uint8_t y, uint8_t valueOffset, const char *clearString, int value) {
  printToStatusDisplay(x, y, clearString);
  printToStatusDisplay(x + valueOffset, y, value);
}

bool Dwelling::printIndicatorToStatusDisplay(uint8_t x, uint8_t y, bool print, const char indicator) {
  _statusDisplay.setCursor(x, y);
  if (print) {
    _statusDisplay.print(indicator);
  }
  else {
    _statusDisplay.print(" ");
  }
  return print;
}

void Dwelling::houseBatteryStatusLight(int tickCount) {
  switch (_electricalStorage.powerLevel()) {
  case PowerNearFull:
  case PowerFull:
    _batteryStatusLight.turnOnGreen();
    break;
  case PowerCritical:
  case PowerLow:
    _batteryStatusLight.turnOnRed();
    break;
  case PowerMiddle:
    _batteryStatusLight.turnOff();
    break;
  }

  // blink red if low but not critical
  // blink green if NearFull but not Full
  if (_electricalStorage.powerLevel() == PowerLow || _electricalStorage.powerLevel() == PowerNearFull) {
    const int blinkSpeedTicks = 5;

    if ((tickCount % blinkSpeedTicks) == 0) {
      if (_batteryStatusLight.isOn()) {
        _batteryStatusLight.turnOff();
      }
      else {
        if (_batteryStatusLight.wasRed()) {
          _batteryStatusLight.turnOnRed();
        }
        else {
          _batteryStatusLight.turnOnGreen();
        }
      }
    }
  }
}

void Dwelling::exteriorMotionDetector(int ticks) {
  if (_intruderAlarm.isOn()) {
    // turn exterior floodlights and alarm indicator on
    if (_electricalStorage.powerLevel() != PowerCritical) {
      _exteriorLights.turnOn();
      _exteriorLightsTurnedOnManually = false;
    }
    if (_intruderAlarm.isOn()) {
      _exteriorAlertLight.turnOn();
    }
  }
  if (_intruderAlarm.isOff()) {
    _exteriorAlertLight.turnOff();
    if (!_exteriorLightsTurnedOnManually) {
      _exteriorLights.turnOff();
      _exteriorLightsTurnedOnManually = false;
    }
  }
}