#ifndef SENSOR_H
#define SENSOR_H
#include "includes.h"

extern VL53L0X distSensor;

void setupSensor();

uint16_t getDistancia();

#endif