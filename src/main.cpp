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
const int ticksPerCharging = 10;              // charging happens every second

// Dwelling Contents
// ButtonPullUp floodlightOverrideButton = ButtonPullUp(floodlightOverridePin);
// ButtonPullUp interiorLightsButton = ButtonPullUp(interiorLightsButtonPin);
// ButtonPullUp intruderAlarmOverrideButton = ButtonPullUp(intruderAlarmOverridePin);
DigitalPinIn floodlightOverrideButton = DigitalPinIn(floodlightOverridePin, DigitalPinIn::withPullup, DigitalPinIn::lowOn);
DigitalPinIn interiorLightsButton = DigitalPinIn(interiorLightsButtonPin, DigitalPinIn::withPullup, DigitalPinIn::lowOn);
DigitalPinIn intruderAlarmOverrideButton = DigitalPinIn(intruderAlarmOverridePin, DigitalPinIn::withPullup, DigitalPinIn::lowOn);

DigitalPinOut exteriorFloodlights = DigitalPinOut(exteriorFloodlightsPin, DigitalPinOut::highOn);
DigitalPinOut exteriorAlertLight = DigitalPinOut(exteriorAlertLightPin, DigitalPinOut::highOn);

DigitalPinIn intruderAlarm = DigitalPinIn(intruderMotionAlarmPin, DigitalPinIn::withoutPullup, DigitalPinIn::highOn);

Buzzer alarmSystem = Buzzer(alarmSystemPWMPin);
DimmableLED interiorLights = DimmableLED(interiorLightsPWMControlPin);
LiquidCrystal_I2C statusDisplay(0x27, 16, 2);
Power electricalStorage = Power(solarArrayAnalogInputPin);

const double interiorLightsPowerUsage = 3.0;

// Forward Declarations
void interiorLighting(void);
void batteryChargingAndUsage(void);
void updateStatusDisplay(void);


// Arduino Setup
void setup() {
  statusDisplay.init();
  statusDisplay.clear();
  statusDisplay.backlight();

  pinMode(intruderMotionAlarmPin, INPUT);

  Serial.begin(9600);
  while (!Serial);
  Serial.println("setup complete");
}


// Arduino Loop
// Instead of using delay(), millis() is used to enforce a timing 'tick' of
// 1/10 of a second (oneTenthOfASecond).
void loop() {
  floodlightOverrideButton.isOn() ? exteriorFloodlights.turnOn() : exteriorFloodlights.turnOff();
  intruderAlarmOverrideButton.isOn() ? exteriorAlertLight.turnOn() : exteriorAlertLight.turnOff();

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