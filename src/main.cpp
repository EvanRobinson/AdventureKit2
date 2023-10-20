/*
  Simulated post-apocalyptic dwelling

  Supports simulated interior lighting (interiorLights)
    controlled by a momentary switch which toggles lights
    on and off
  Simulated electrical storage (electricalStorage) and
    solar charger (power level supplied by photoresistor)
  Lighting now simulates a power drop when on
*/
#include <Arduino.h>
#include <Keypad.h>
#include <math.h>
#include <Wire.h>
#include "button.h"
#include "DigitalPinIO.h"
#include "led.h"
#include "LiquidCrystal_I2C.h"
#include "passive_buzzer.h"
#include "photoresistor.h"
#include "pins.h"
#include "potentiometer.h"
#include "power.h"

// Timing constants
const unsigned long oneTenthOfASecond = 100L; // one 'tick'
const int ticksPerLighting = 1;               // lighting input happens every tick
const int ticksPerMotionSensor = 1;           // motion sensor checked every tick
const int ticksPerCharging = 10;              // charging happens every second

// Dwelling Contents
DigitalPinIn floodlightOverrideButton = DigitalPinIn(floodlightOverridePin, DigitalPinIO::withPullup, DigitalPinIO::lowOn);
DigitalPinIn interiorLightsButton = DigitalPinIn(interiorLightsButtonPin, DigitalPinIO::withPullup, DigitalPinIO::lowOn);
DigitalPinIn intruderAlarmOverrideButton = DigitalPinIn(intruderAlarmOverridePin, DigitalPinIO::withPullup, DigitalPinIO::lowOn);
DigitalPinOut exteriorFloodlights = DigitalPinOut(exteriorFloodlightsPin, DigitalPinIO::highOn);
DigitalPinOut exteriorAlertLight = DigitalPinOut(exteriorAlertLightPin, DigitalPinIO::highOn);
DigitalPinIn intruderAlarm = DigitalPinIn(intruderMotionAlarmPin, DigitalPinIO::withoutPullup, DigitalPinIO::highOn);
Buzzer alarmSystem = Buzzer(alarmSystemPWMPin);
DimmableLED interiorLights = DimmableLED(interiorLightsPWMControlPin);
LiquidCrystal_I2C statusDisplay(0x27, 16, 2);
Power electricalStorage = Power(solarArrayAnalogInputPin);

// keypad
const byte keypadRows = 4;
const byte keypadColumns = 4;

const char keys[keypadRows][keypadColumns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}  
};
byte rowPins[keypadRows] = {keypad00, keypad01, keypad02, keypad03};
byte columnPins[keypadColumns] = {keypad04, keypad05, keypad06, keypad07};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, keypadRows, keypadColumns);

const double interiorLightsPowerUsage = 3.0;

// Forward Declarations
void batteryChargingAndUsage(void);
void interiorLighting(void);
void motionSensor(int ticks);
void updateStatusDisplay(void);


// Arduino Setup
void setup() {
  statusDisplay.init();
  statusDisplay.clear();
  statusDisplay.backlight();

  Serial.begin(9600);
  while (!Serial);
  Serial.println("setup complete");
}


// Arduino Loop
// Instead of using delay(), millis() is used to enforce a timing 'tick' of
// 1/10 of a second (oneTenthOfASecond).
void loop() {
  static int tickCount = 0;
  static unsigned long previousMillis = 0L;
  unsigned long currentMillis = millis();

  if ((currentMillis - previousMillis) < oneTenthOfASecond) {
    return;
  }

  previousMillis = currentMillis;
  tickCount++;
  alarmSystem.tick();
  electricalStorage.tick();

  updateStatusDisplay();

  if ((tickCount % ticksPerLighting) == 0) {
    interiorLighting();
  }

  if ((tickCount % ticksPerCharging) == 0) {
    batteryChargingAndUsage();
  }

  if ((tickCount % ticksPerMotionSensor) == 0) {
    motionSensor(tickCount);
  }  
}

// Local Functions
void interiorLighting() {
  // Turn interiorLights on and off using button
  if (interiorLightsButton.isOn()) {
    if (interiorLightsButton.hasChanged()) {
      if (interiorLights.isOn()) {
        interiorLights.turnOff();
      }
      else {
        interiorLights.turnOn();
      }
    }
  }

  // manage lights depending upon current power levels
  if (interiorLights.isOn()) {
    if (electricalStorage.isCritical()) {
      alarmSystem.alarm(power_critical);
      interiorLights.dimmerLevel(2); // TBD: Fix magic number
      interiorLights.turnOn();
    }
    else if (electricalStorage.isLow()) {
      alarmSystem.alarm(power_low);
      interiorLights.dimmerLevel(63); // TBD: Fix magic number
      interiorLights.turnOn();
    }
    else {
      if (interiorLights.isOn()) {
        interiorLights.dimmerLevel(255); // TBD: Fix magic number
        interiorLights.turnOn();
      }
    }
  }
}

void batteryChargingAndUsage() {
  static double previousBatteryLevel = electricalStorage.batteryLevel();
  
  electricalStorage.chargeBattery();

  // account for interiorLights power usage
  if (interiorLights.isOn()) {
    int interiorLightsPowerUsage = 1;
    electricalStorage.usePower(interiorLightsPowerUsage);
  }

  // send battery power level to serial
  // TBD: Remove when this info is sent to hardware display instead of Serial
  if (previousBatteryLevel != electricalStorage.batteryLevel()) {
    electricalStorage.showStatus(); // manage indicator lights & alarms
    electricalStorage.showStatus(statusDisplay);  // manage LCD display
  }
  previousBatteryLevel = electricalStorage.batteryLevel();
}

void updateStatusDisplay(void) {
  statusDisplay.setCursor(14,0);
  if (intruderAlarm.isOn()) {
    statusDisplay.print("*");  }
  else {
    statusDisplay.print(" ");
  }

  if (floodlightOverrideButton.isOn()) {
    statusDisplay.setCursor(15,0);
    statusDisplay.print("*");
  }
  else {
    statusDisplay.setCursor(15,0);
    statusDisplay.print(" ");
  }

  if (intruderAlarmOverrideButton.isOn()) {
    statusDisplay.setCursor(15,1);
    statusDisplay.print("*");
  }
  else {
    statusDisplay.setCursor(15,1);
    statusDisplay.print(" ");
  }
}

const int intruderAlarmBlinkTime = 20;

void motionSensor(int ticks) {
  static bool intruderAlarmLightOn = false;
  static int intruderAlarmLightOnTime = 0;
  static bool intruderAlarmBlinking = false;
  static int intruderAlarmBlinkStartTime = 0;

  if (exteriorAlertLight.isOn() && intruderAlarmOverrideButton.isOn()) {
    exteriorAlertLight.turnOff();
    intruderAlarmLightOn = false;
    intruderAlarmBlinking = false;
  }
  if (exteriorFloodlights.isOn() && floodlightOverrideButton.isOn()) {
    exteriorFloodlights.turnOff();
  }

  if (intruderAlarm.isOn()) {
    // turn exterior floodlights and alarm indicator on
    if (!intruderAlarmLightOn && intruderAlarm.isOn()) {
      intruderAlarmLightOn = true;
      intruderAlarmLightOnTime = ticks;
      exteriorFloodlights.turnOn();
      exteriorAlertLight.turnOn();
    }
    return;
  }

  if (intruderAlarmLightOn && !intruderAlarmBlinking) {
    if ((ticks - intruderAlarmLightOnTime) > intruderAlarmBlinkTime) {
      intruderAlarmBlinking = true;
      intruderAlarmBlinkStartTime = ticks;
    }
  }

  if (intruderAlarmBlinking) {
    int interval = ticks - intruderAlarmBlinkStartTime;
    if (interval % 2 == 0) {
      exteriorAlertLight.toggle();
    }
  }
}