#include "sensor.h"

VL53L0X distSensor;

void setupSensor()
{
    Wire.begin(I2C_Pins::SDA, I2C_Pins::SCL);
    distSensor.setTimeout(500);

    if (!distSensor.init())
    {
        Serial.println("VL53L0X nao encontrado!");
        while (true)
            delay(1000);
    }
    distSensor.startContinuous();
}

uint16_t getDistancia()
{
    uint16_t mm = distSensor.readRangeContinuousMillimeters();

    if (distSensor.timeoutOccurred() || mm > 2000)
    {
        return 0;
    }
    return mm;
}
