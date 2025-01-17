#ifndef LOCKCONTROL_H
#define LOCKCONTROL_H

#include "Particle.h"

void setupLockControl();
void SafteyTurnOffLockPin(int lock_pin_status);
int ControlLockOfParcelBox(String command);
int getLockPinStatus();

#endif // LOCKCONTROL_H