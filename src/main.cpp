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
#include "button.h"
#include "led.h"
#include "passive_buzzer.h"
#include "photoresistor.h"
#include "potentiometer.h"
#include "power.h"
// Hardware values
const uint8_t whiteLEDPWMControlPin = 13;
const uint8_t buttonInputPin = 24;
const uint8_t photoResistorInputPin = 0;
const uint8_t potentiometerAnalogInputPin = 15;
const uint8_t buzzerPWMPin = 13;

// Timing constants
const unsigned long oneTenthOfASecond = 100L; // one 'tick'
const int ticksPerLighting = 1;               // lighting input happens every tick
const int ticksPerCharging = 10;              // charging happens every second

// Dwelling Contents
Button interiorLightsButton = Button(buttonInputPin);
DimmableLED interiorLights = DimmableLED(whiteLEDPWMControlPin);
Power electricalStorage = Power(photoResistorInputPin);
Buzzer alarmSystem = Buzzer(buzzerPWMPin);
Potentiometer interiorLightsDimmer = Potentiometer(potentiometerAnalogInputPin);

const double interiorLightsPowerUsage = 3.0;

// Forward Declarations
void interiorLighting(void);
void batteryChargingAndUsage(void);



// Arduino Setup
void setup() {
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

  int potValue = int(interiorLightsDimmer.readScaledTo(0,255));
  interiorLights.dimmerLevel(potValue);

  if ((currentMillis - previousMillis) < oneTenthOfASecond) {
    return;
  }
  previousMillis = currentMillis;
  tickCount++;
  alarmSystem.tick();
  electricalStorage.tick();

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
  if (interiorLightsButton.isPressed()) {
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
      interiorLights.dimmerLevel(2);
      interiorLights.turnOn();
    }
    else if (electricalStorage.isLow()) {
      alarmSystem.alarm(power_low);
      interiorLights.dimmerLevel(63);
      interiorLights.turnOn();
    }
    else {
      if (interiorLights.isOn()) {
        interiorLights.dimmerLevel(255);
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
      electricalStorage.showStatus();
  }
  previousBatteryLevel = electricalStorage.batteryLevel();
}
