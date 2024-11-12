
#include "Particle.h"
#include "Adafruit_LSM9DS1.h"
#include "Adafruit_Sensor.h"

// Create an instance of the sensor
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

// Define the rotation values for X, Y, Z axes
float rotationX, rotationY, rotationZ;
int counter = 0;
int resetCounter(String args);
int incrementCounter(String args);
// Threshold value to detect movement (adjust as needed)
float rotationThreshold = 0.1; // Set a threshold for rotation (in degrees per second)

void setup()
{
  Particle.variable("counter", counter);

  // Cloud-Funktion registrieren
  Particle.function("resetC", resetCounter);
  Particle.function("incrementVariable", incrementCounter);

  Serial.begin(9600); // Start the serial communication
  delay(1000);        // Wait for the serial communication to initialize

  // Initialize the sensor
  if (!lsm.begin())
  {
    Serial.println("Error: LSM9DS1 sensor not found!");
    while (1)
      ; // Stop execution if the sensor is not found
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

  // Get the rotation (gyroscope) values in degrees per second (dps)
  rotationX = lsm.gyroData.x;
  rotationY = lsm.gyroData.y;
  rotationZ = lsm.gyroData.z;

  // Print the rotation values to the serial monitor for debugging
  Serial.print("Rotation X: ");
  Serial.print(rotationX);
  Serial.print(" dps, Y: ");
  Serial.print(rotationY);
  Serial.print(" dps, Z: ");
  Serial.println(rotationZ);

  // Check if any of the rotation values exceed the threshold
  checkRotationValues(rotationX, rotationY, rotationZ);
 

  delay(1000); // Delay for 1 second before the next reading
}

int incrementCounter(String args)
{
  counter++;
  return 0;
}

int resetCounter(String args)
{
  counter = 0; 
  return 0;
}