#include "LockControl.h"
#include "Particle.h"

const int transistor_lock_pin = D3;

void setupLockControl() {
    pinMode(transistor_lock_pin, OUTPUT);
    Particle.function("ControlLockOfParcelBox", ControlLockOfParcelBox);
}

void SafteyTurnOffLockPin(int lock_pin_status) {
    if (lock_pin_status == HIGH) {
        digitalWrite(transistor_lock_pin, LOW);
    }
}

int ControlLockOfParcelBox(String command) {
    if (command == "openbox") {
        digitalWrite(transistor_lock_pin, HIGH);
        delay(100);
        digitalWrite(transistor_lock_pin, LOW);
        return 1;
    }
    return -1;
}

int getLockPinStatus() {
    return digitalRead(transistor_lock_pin);
}