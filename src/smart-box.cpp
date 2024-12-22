#include "Particle.h"
#include "Adafruit_LSM9DS1.h"
#include "Adafruit_Sensor.h"
#include "smart-box.h"
#include "led.h"
#include "HX711ADC.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

#define ACC_THRESHOLD 2.0  // Beschleunigung in g
#define GYRO_THRESHOLD 100 // Drehgeschwindigkeit in dps
#define HX711_DT D5
#define HX711_CLK D6
#define MAGNETIC_CONTACT_PIN D7

int transistor_lock_pin = D3;

int box_cover_status = 0 ;
// Create an instance of the sensor
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
HX711ADC scale;

// Create an instance of the LED class
// LED redled(D6, "red");
// LED greenled(D7, "green");



// Define the rotation values for X, Y, Z axes
float rotationX,rotationY, rotationZ;
int counter = 0;
// Threshold value to detect movement (adjust as needed)
float rotationThreshold = 0.1; // Set a threshold for rotation (in degrees per second)

// int getLedStatusInMain(String command)
// {
//     if (command == "red")
//     {
//         return redled.getStatus() ? 1 : 0;
//     }
//     else if (command == "green")
//     {
//         return greenled.getStatus() ? 1 : 0;
//     }
//     else return -1;
// }

// int controlLedInMain(String command)
// {
//     if (command.indexOf("red") != -1)
//     {
//         return redled.ledControl(redled, command);
//     }
//     else if (command.indexOf("green") != -1)
//     {
//         return greenled.ledControl(greenled, command);
//     }
//     else
//     {
//         return -1;
//     }
// }

int ControlLockOfParcelBox(String command)
{
    if (command == "openbox")
    {
        digitalWrite(transistor_lock_pin, HIGH);
        delay(100);
        digitalWrite(transistor_lock_pin, LOW);
        return 1;
    }

    else
    {
        return -1;
    }
}

int GetBoxStatus(String command){
    
    if (command == "boxstatus"){
        if (box_cover_status == HIGH)
        {
            return 0;
        }
        else if (box_cover_status == LOW)
        {
            return 1;
        }
    }
   
    
    return -1;
    
}


void setup()
{

    Serial.begin(9600); // Start the serial communication
    delay(1000);        // Wait for the serial communication to initialize

    scale.begin(HX711_DT,HX711_CLK);           // HX711 initialisieren

    Serial.println("Kalibrierung...");
    delay(2000);

    // Nullpunkt setzen
    scale.set_scale(); // Standardkalibrierung verwenden
    scale.tare();      // Aktuelles Gewicht als Null setzen
    Serial.println("Kalibrierung abgeschlossen!");

    pinMode(transistor_lock_pin, OUTPUT); // sets the Pin to control the lock mechanism
    pinMode(MAGNETIC_CONTACT_PIN, INPUT_PULLUP);
    // controlLedInMain("red_on");
    Serial.println("SETUP FUNCTION MAINNN");

    Particle.function("ControlLockOfParcelBox", ControlLockOfParcelBox);
    Particle.function("GetBoxStatus", GetBoxStatus);
    
    // WebHook
    
    // Particle.function("controlLedInMain", controlLedInMain);
    //  Particle.variable("redLed", redled.getLedStatus());
    //  Particle.function("getLedStatus", getLedStatus);

    // Initialize the sensor
    //    if (!lsm.begin())
    //    {
    //      Serial.println("Error: LSM9DS1 sensor not found!");
    //      while (1); // Stop execution if the sensor is not found
    //    }
    //    Serial.println("LSM9DS1 sensor initialized successfully!");
}

// Function to check if any rotation values exceed the threshold
void checkRotationValues(float x, float y, float z)
{
    if (fabs(x) > rotationThreshold || fabs(y) > rotationThreshold || fabs(z) > rotationThreshold)
    {
        Serial.println("Paket geklaut!"); // Output the message if movement is detected
    }
}

void SafteyTurnOffLockPin(int lock_pin_status){
    if(lock_pin_status == HIGH){
        digitalWrite(transistor_lock_pin, LOW);
    }
    else {
        return;
    }
}

void loop()
{

    int lock_pin_status = digitalRead(transistor_lock_pin);
    Serial.println(lock_pin_status);        // Gibt 1 (HIGH) oder 0 (LOW) aus
    SafteyTurnOffLockPin(lock_pin_status);

    int contactState = digitalRead(MAGNETIC_CONTACT_PIN); // Status des Magnetkontakts lesen
    
    if(contactState != box_cover_status) {
        box_cover_status = contactState;
        Particle.publish("boxstatus", String(box_cover_status), PRIVATE);
    }
    // read the value of the box cover status
    box_cover_status = contactState;

    if (contactState == HIGH)
    {
        Serial.println("BOX COVER ARE OPEN");
    }
    else
    {
        Serial.println("BOX COVER ARE CLOSED");
    }

    
    

    float weight = scale.get_units(10); // Durchschnitt von 10 Messungen
    Serial.print("Gewicht: ");
    Serial.print(weight);
    Serial.println(" kg");

    delay(200); // Warte 1 Sekunde
    //     // Fetch the sensor data (accelerometer, gyroscope, and magnetometer)
    //        lsm.read();
    //        sensors_event_t accelEvent, gyroEvent, magEvent, tempEvent;
    //        lsm.getEvent(&accelEvent, &gyroEvent, &magEvent, &tempEvent);

    //        // Get the rotation (gyroscope) values in degrees per second (dps)
    //     float accelX = accelEvent.acceleration.x;
    //     float accelY = accelEvent.acceleration.y;
    //     float accelZ = accelEvent.acceleration.z;

    //     float gyroX = gyroEvent.gyro.x;
    //     float gyroY = gyroEvent.gyro.y;
    //     float gyroZ = gyroEvent.gyro.z;

    //    float dynamicAccelX = accelX;
    //    float dynamicAccelY = accelY;
    //    float dynamicAccelZ = accelZ - 9.81; // Gravitation entfernen

    //    float totalDynamicAccel = sqrt(dynamicAccelX * dynamicAccelX +
    //                                  dynamicAccelY * dynamicAccelY +
    //                                  dynamicAccelZ * dynamicAccelZ);

    //   // Prüfe auf Schwellenüberschreitung
    //   if (totalDynamicAccel > ACC_THRESHOLD || abs(gyroX) > GYRO_THRESHOLD || abs(gyroY) > GYRO_THRESHOLD || abs(gyroZ) > GYRO_THRESHOLD) {
    //     Serial.println("Bewegung erkannt! ALARM!");
    //     Particle.publish("ALARM", "ALARM!");
    //     delay(1000*2);
    //     Serial.println(totalDynamicAccel);
    //     Serial.println(gyroX);
    //     Serial.println(gyroY);
    //     Serial.println(gyroZ);
    //   } else {
    //     Serial.println("Kein Alarm mehr!");
    //     delay(1000);
    //   }
}
