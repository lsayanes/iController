
#include <stdint.h>
#include "switch.hpp"
#include "midi.hpp"
#include "device.hpp"
#include "Fractal.hpp"

static constexpr int32_t   debugLed { LED_BUILTIN };


Fractal fractal{ 1 };

 
void setup() 
{

  Serial.begin(fractal.baud); 

  pinMode(debugLed, OUTPUT);
  digitalWrite(debugLed, LOW);

  fractal.create();
}


void loop() 
{
  fractal.sceneDemo();
}
