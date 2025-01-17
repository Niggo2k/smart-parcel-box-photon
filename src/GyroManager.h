#ifndef GYROMANAGER_H
#define GYROMANAGER_H

#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include "Particle.h"

void setupGyroManager();
void readSensors();
void checkForMovement();

extern Adafruit_LSM9DS1 lsm;
extern float accelX, accelY, accelZ;
extern float gyroX, gyroY, gyroZ;
extern float dynamicAccelX, dynamicAccelY, dynamicAccelZ;
extern float totalDynamicAccel;
extern const float ACC_THRESHOLD;
extern const float GYRO_THRESHOLD;

#endif // GYROMANAGER_H