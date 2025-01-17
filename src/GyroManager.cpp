#include "GyroManager.h"
#include "Particle.h"

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
float accelX, accelY, accelZ;
float gyroX, gyroY, gyroZ;
float dynamicAccelX, dynamicAccelY, dynamicAccelZ;
float totalDynamicAccel;
const float ACC_THRESHOLD = 2.0;  // Adjust as needed
const float GYRO_THRESHOLD = 100; // Adjust as needed

void setupGyroManager() {
    if (!lsm.begin()) {
        Serial.println("Failed to initialize LSM9DS1!");
        while (1);
    }
    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
}

void readSensors() {
    lsm.read();
    sensors_event_t accelEvent, gyroEvent, magEvent, tempEvent;
    lsm.getEvent(&accelEvent, &gyroEvent, &magEvent, &tempEvent);

    accelX = accelEvent.acceleration.x;
    accelY = accelEvent.acceleration.y;
    accelZ = accelEvent.acceleration.z;

    gyroX = gyroEvent.gyro.x;
    gyroY = gyroEvent.gyro.y;
    gyroZ = gyroEvent.gyro.z;

    dynamicAccelX = accelX;
    dynamicAccelY = accelY;
    dynamicAccelZ = accelZ - 9.81; // Remove gravity

    totalDynamicAccel = sqrt(dynamicAccelX * dynamicAccelX +
                             dynamicAccelY * dynamicAccelY +
                             dynamicAccelZ * dynamicAccelZ);

    checkForMovement();
}

void checkForMovement() {
    if (totalDynamicAccel > ACC_THRESHOLD || abs(gyroX) > GYRO_THRESHOLD || abs(gyroY) > GYRO_THRESHOLD || abs(gyroZ) > GYRO_THRESHOLD) {
        Serial.println("Movement detected! ALARM!");
        Particle.publish("ALARM", "ALARM");
        delay(2000);
        Serial.println(totalDynamicAccel);
        Serial.println(gyroX);
        Serial.println(gyroY);
        Serial.println(gyroZ);
    } else {
        Serial.println("No alarm.");
        delay(1000);
    }
}