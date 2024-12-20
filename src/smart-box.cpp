#include "Particle.h"
#include "Adafruit_LSM9DS1.h"
#include "Adafruit_Sensor.h"
#include "smart-box.h"
#include "led.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

#define ACC_THRESHOLD 2.0  // Beschleunigung in g
#define GYRO_THRESHOLD 100 // Drehgeschwindigkeit in dps

// Create an instance of the sensor
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
// Create an instance of the LED class
LED redled(D6, "red");
LED greenled(D7, "green");
// Define the rotation values for X, Y, Z axes
float rotationX,
rotationY, rotationZ;
int counter = 0;
// Threshold value to detect movement (adjust as needed)
float rotationThreshold = 0.1; // Set a threshold for rotation (in degrees per second)

int getLedStatusInMain(String command)
{
    if (command == "red")
    {
        return redled.getStatus() ? 1 : 0;
    }
    else if (command == "green")
    {
        return greenled.getStatus() ? 1 : 0;
    }
    else return -1;
}

int controlLedInMain(String command)
{
    if (command.indexOf("red") != -1)
    {
        return redled.ledControl(redled, command);
    }
    else if (command.indexOf("green") != -1)
    {
        return greenled.ledControl(greenled, command);
    }
    else
    {
        return -1;
    }
}

void setup()
{

    Serial.begin(9600); // Start the serial communication
    delay(1000);        // Wait for the serial communication to initialize
    controlLedInMain("red_on");
    Serial.println("SETUP FUNCTION MAINNN");
    // Particle.variable("redLed", redled.getLedStatus());
    Particle.function("controlLedInMain", controlLedInMain);
    // Particle.function("getLedStatus", getLedStatus);

    // Initialize the sensor
       if (!lsm.begin())
       {
         Serial.println("Error: LSM9DS1 sensor not found!");
         while (1); // Stop execution if the sensor is not found
       }
       Serial.println("LSM9DS1 sensor initialized successfully!");
}

// Function to check if any rotation values exceed the threshold
 void checkRotationValues(float x, float y, float z)
 {
   if (fabs(x) > rotationThreshold || fabs(y) > rotationThreshold || fabs(z) > rotationThreshold)
   {
     Serial.println("Paket geklaut!"); // Output the message if movement is detected
   }
 }

void loop()
{
    // Fetch the sensor data (accelerometer, gyroscope, and magnetometer)
       lsm.read();
       sensors_event_t accelEvent, gyroEvent, magEvent, tempEvent;
       lsm.getEvent(&accelEvent, &gyroEvent, &magEvent, &tempEvent);

       // Get the rotation (gyroscope) values in degrees per second (dps)
    float accelX = accelEvent.acceleration.x;
    float accelY = accelEvent.acceleration.y;
    float accelZ = accelEvent.acceleration.z;

    float gyroX = gyroEvent.gyro.x;
    float gyroY = gyroEvent.gyro.y;
    float gyroZ = gyroEvent.gyro.z;

   float dynamicAccelX = accelX;
   float dynamicAccelY = accelY;
   float dynamicAccelZ = accelZ - 9.81; // Gravitation entfernen

   float totalDynamicAccel = sqrt(dynamicAccelX * dynamicAccelX +
                                 dynamicAccelY * dynamicAccelY +
                                 dynamicAccelZ * dynamicAccelZ);

  // Prüfe auf Schwellenüberschreitung
  if (totalDynamicAccel > ACC_THRESHOLD || abs(gyroX) > GYRO_THRESHOLD || abs(gyroY) > GYRO_THRESHOLD || abs(gyroZ) > GYRO_THRESHOLD) {
    Serial.println("Bewegung erkannt! ALARM!");
    Particle.publish("ALARM", "ALARM!");
    delay(1000*2);
    Serial.println(totalDynamicAccel);
    Serial.println(gyroX);
    Serial.println(gyroY);
    Serial.println(gyroZ);
  } else {
    Serial.println("Kein Alarm mehr!");
    delay(1000);
  }

  delay(50);
}
