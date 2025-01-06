#ifndef LOADCELLMANAGER_H
#define LOADCELLMANAGER_H

#include "HX711ADC.h"
#include "Particle.h"

extern HX711ADC LoadCell;
extern double globalWeight;

void setupLoadCell();
int GetWeight(String command);
int calibrateScale(String command);
int TareWeight(String command);

#endif // LOADCELLMANAGER_H