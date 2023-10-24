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

#include "dwelling.h"

// Timing constants
const unsigned long oneTenthOfASecond = 100L; // one 'tick'

Dwelling dwelling = Dwelling();

// Arduino Setup
void setup() {
  dwelling.init();

  Serial.begin(115200);
  while (!Serial)
    ;

  dwelling.unlock();
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

  dwelling.tick(tickCount);
}