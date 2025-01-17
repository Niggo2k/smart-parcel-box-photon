#include "BoxStatus.h"
#include "Particle.h"

#define MAGNETIC_CONTACT_PIN D7
int box_cover_status = LOW;

void setupBoxStatus() {
    pinMode(MAGNETIC_CONTACT_PIN, INPUT_PULLUP);
    Particle.variable("boxstatus", box_cover_status);
    Particle.function("GetBoxStatus", GetBoxStatus);
}

int GetBoxStatus(String command) {
    return box_cover_status;
}

int readBoxCoverStatus() {
    return digitalRead(MAGNETIC_CONTACT_PIN);
}

void publishBoxStatus(int contactState) {
    if (contactState != box_cover_status) {
        box_cover_status = contactState;
        Particle.publish("boxstatus", String(box_cover_status), PRIVATE);
    }
    box_cover_status = contactState;
}