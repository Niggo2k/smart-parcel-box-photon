#include "led.h"
#include "Particle.h"
#include <iostream>
#include <sstream>
using namespace std;

// Konstruktor: Initialisiert die LED am angegebenen Pin
LED::LED(int pin, String _name) : pin(pin), name(_name), status(false)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // LED startet ausgeschaltet
}

// Methode zum Steuern der LED
int LED::ledControl(LED &led, String command)
{
    string ledName, ledMode;
    istringstream stream(command.c_str());
    getline(stream, ledName, '_'); // Extract the LED name (e.g., "red" or "green")
    getline(stream, ledMode);      // Extract the LED mode (e.g., "on" or "off")

    // Check if the LED name is "red"
    if (ledName == "red")
    {
        if (ledMode == "on")
        {
            digitalWrite(led.getPin(), HIGH); // Turn on the red LED
            led.setStatus(true);
            return 1;
        }
        else if (ledMode == "off")
        {
            digitalWrite(led.getPin(), LOW); // Turn off the red LED
            led.setStatus(false);
            return 0;
        }
    }
    // Check if the LED name is "green"
    else if (ledName == "green")
    {
        if (ledMode == "on")
        {
            digitalWrite(led.getPin(), HIGH); // Turn on the green LED
            led.setStatus(true);
            return 1;
        }
        else if (ledMode == "off")
        {
            digitalWrite(led.getPin(), LOW); // Turn off the green LED
            led.setStatus(false);
            return 0;
        }
    }
    // If the LED name is neither "red" nor "green", return -1
    else
    {
        return -1;
    }
    return -1; // Default return value if no conditions are met
}

// Methode zur Abfrage des LED-Status
int LED::getLedStatus(String command) const
{
    return status ? 0 : -1; // Gibt 0 für "An", -1 für "Aus"
}

// Getter-Methode für den Pin
int LED::getPin() const
{
    return pin;
}

// Setter-Methode für den Pin
void LED::setPin(int newPin)
{
    pin = newPin;
    pinMode(pin, OUTPUT);   // Konfiguriere den neuen Pin als Ausgang
    digitalWrite(pin, LOW); // Setze die LED auf "Aus"
}

// Getter-Methode für den Status
bool LED::getStatus() const
{
    return status;
}

// Setter-Methode für den Status
void LED::setStatus(bool newStatus)
{
    status = newStatus;
    digitalWrite(pin, status ? HIGH : LOW); // Schalte die LED entsprechend dem neuen Status
}

String LED::getName() const{
    return name;
}
