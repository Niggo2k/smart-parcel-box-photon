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

// Create an instance of the sensor
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
// Create an instance of the LED class
LED redled(D7, "red");
LED greenled(D6, "green");
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

    //   // Initialize the sensor
    //   if (!lsm.begin())
    //   {
    //     Serial.println("Error: LSM9DS1 sensor not found!");
    //     while (1); // Stop execution if the sensor is not found
    //   }
    //   Serial.println("LSM9DS1 sensor initialized successfully!");
}

// Function to check if any rotation values exceed the threshold
// void checkRotationValues(float x, float y, float z)
// {
//   if (fabs(x) > rotationThreshold || fabs(y) > rotationThreshold || fabs(z) > rotationThreshold)
//   {
//     Serial.println("Paket geklaut!"); // Output the message if movement is detected
//   }
// }

void loop()
{
    Log.info("Sending Hello World to the cloud!");
    Particle.publish("Hello world!");
    delay(10 * 1000); // milliseconds and blocking - see docs for more info!
    //   // Fetch the sensor data (accelerometer, gyroscope, and magnetometer)
    //   lsm.read();

    //   // Get the rotation (gyroscope) values in degrees per second (dps)
    //   rotationX = lsm.gyroData.x;
    //   rotationY = lsm.gyroData.y;
    //   rotationZ = lsm.gyroData.z;

    //   // Print the rotation values to the serial monitor for debugging
    //   Serial.print("Rotation X: ");
    //   Serial.print(rotationX);
    //   Serial.print(" dps, Y: ");
    //   Serial.print(rotationY);
    //   Serial.print(" dps, Z: ");
    //   Serial.println(rotationZ);

    //   // Check if any of the rotation values exceed the threshold
    //   checkRotationValues(rotationX, rotationY, rotationZ);

    //   delay(1000); // Delay for 1 second before the next reading
}
