#ifndef BOXSTATUS_H
#define BOXSTATUS_H

#include "Particle.h"

extern int box_cover_status;

void setupBoxStatus();
int GetBoxStatus(String command);
int readBoxCoverStatus();
void publishBoxStatus(int contactState);

#endif // BOXSTATUS_H