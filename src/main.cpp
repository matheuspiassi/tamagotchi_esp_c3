#include <Arduino.h>
#include "includes.h"

void setup()
{
  Serial.begin(115200);
  setupDisplay();
  setupMPU();
  
}

void loop(){

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

    Serial.printf("Acel m/s2  X:%7.2f  Y:%7.2f  Z:%7.2f  |  ",
                  a.acceleration.x, a.acceleration.y, a.acceleration.z);
    Serial.printf("Giro rad/s  X:%6.2f  Y:%6.2f  Z:%6.2f  |  ",
                  g.gyro.x, g.gyro.y, g.gyro.z);
    Serial.printf("Temp %5.1f C\n", temp.temperature);

    delay(200);
}