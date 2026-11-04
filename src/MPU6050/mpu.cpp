#include "mpu.h"
constexpr uint8_t MPU_ADDR = 0x68;
Adafruit_MPU6050 mpu;

void setupMPU()
{
    if(!mpu.begin(MPU_ADDR, &Wire))
    {
        Serial.println("MPU não encontrado!");
        while(true) delay(1000);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU Pronto");
}