#include "Particle.h"
#include "smart-box.h"
#include "LoadCellManager.h"
#include "LockControl.h"
#include "BoxStatus.h"
#include "GyroManager.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);
int transistor_lock_pin = D3;

void setup()
{

    Serial.begin(9600); // Start the serial communication
    delay(1000);  // Wait for the serial communication to initialize
    Serial.println();
    Serial.println("Starting...");

    setupLoadCell();
    setupLockControl();
    setupBoxStatus();
    setupGyroManager();
    Serial.println("Readings:");
}



void loop()
{
    int lock_pin_status = getLockPinStatus();
    SafteyTurnOffLockPin(lock_pin_status);

    int contactState = readBoxCoverStatus();  // Read box cover status
    publishBoxStatus(contactState);  // Publish box status

    // Call the GetWeight function
    GetWeight("weight");

    // Print the rounded weight
    Serial.print("Weight: ");
    Serial.print(globalWeight, 1);  // Print the weight with 1 decimal place
    Serial.println(" kg");
    char weightStr[10];
    snprintf(weightStr, sizeof(weightStr), "%.1f", globalWeight);


    readSensors();
    // Optional: Add a delay if needed
    delay(1000);  // Wait for 1 second
}
