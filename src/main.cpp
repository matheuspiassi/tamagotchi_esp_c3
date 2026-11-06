#include <Arduino.h>
#include "includes.h"

void setup()
{
  Serial.begin(115200);
  setupDisplay();
  setupMPU();
  // setupSensor();
  setupEyes();
  // eyesSetAuto(false);   
  // eyesLookCenter();
}

void loop(){
  updateEyes();
  eyesSetAuto(true);
}