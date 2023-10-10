/*
  Simulated post-apocalyptic dwelling

  Supports simulated interior lighting (interiorLights)
    controlled by a momentary switch (interiorLightsButton)
    which toggles lights on and off
*/
#include <Arduino.h>
#include "button.h"
#include "led.h"

// Hardware values
const uint8_t whiteLEDControlPin = 22;
const uint8_t buttonInputPin = 24;

// Dwelling Contents
Button interiorLightsButton = Button(buttonInputPin);
LED interiorLights = LED(whiteLEDControlPin);
 
// Forward Declarations
void interiorLighting(void);

// Arduino Setup
void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
}

// Arduino Loop
void loop() {
  // Loop every 1/10th seconds
  delay(100);

  interiorLighting();
}

// Local Functions
void interiorLighting() {
  // Turn interiorLights on and off
  if (interiorLightsButton.isPressed()) {
    if (interiorLightsButton.hasChanged()) {
        if (interiorLights.isOn()) {
          Serial.println("Lights Off");
          interiorLights.turnOff();
        }
        else {
          Serial.println("Lights On");
          interiorLights.turnOn();
        }
    }
  }
}
