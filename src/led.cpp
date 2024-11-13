#include "led.h"
#include "Particle.h"

// Konstruktor: Initialisiert die LED am angegebenen Pin
LED::LED(int pin) : pin(pin), status(false)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // LED startet ausgeschaltet
}

// Methode zum Steuern der LED
int LED::ledControl(String command)
{
    if (command == "On")
    {
        digitalWrite(pin, HIGH);
        status = true;
        return 0; // Erfolg
    }
    else if (command == "Off")
    {
        digitalWrite(pin, LOW);
        status = false;
        return 0; // Erfolg
    } 
    return -1;
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
