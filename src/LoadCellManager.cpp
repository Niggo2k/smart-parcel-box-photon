#include "LoadCellManager.h"
#include "Particle.h"

double globalWeight = 0.0;
double previousWeight = 0.0;
double weight = 0.0;
int consecutiveCount = 0;
#define HX711_DT D6
#define HX711_CLK D5

HX711ADC LoadCell(HX711_DT, HX711_CLK);

void setupLoadCell() {
    LoadCell.begin();
    LoadCell.tare();
    LoadCell.set_scale(21.643911f);
    Particle.variable("weight", globalWeight);
    Particle.function("GetWeight", GetWeight);
    Particle.function("CalibrateScale", calibrateScale);
    Particle.function("TareWeight", TareWeight);
}

int GetWeight(String command) {
    float weight = LoadCell.get_units(10);
    Serial.println(weight);
    float weightKg = weight / 1000.0;
    globalWeight = round(weightKg * 10) / 10.0;
    checkWeightChange();
    return 1;
}

int TareWeight(String command) {
    LoadCell.tare();
    return 1;
}

int calibrateScale(String command) {
    Serial.println("Starting calibration...");
    LoadCell.set_scale();
    LoadCell.tare();
    Serial.println("Place a known weight on the scale.");
    delay(5000);
    float knownWeight = 1084.0;
    float reading = LoadCell.get_units(10);
    float scaleFactor = reading / knownWeight;
    LoadCell.set_scale(scaleFactor);
    Serial.println("Scale factor set to: " + String(scaleFactor));
    Serial.println("Calibration complete.");
    return 1;
}

void checkWeightChange() {
    if (abs(globalWeight - previousWeight) > 0.2) {
        consecutiveCount++;
    }else {
        consecutiveCount = 0;
    }

    if (consecutiveCount >= 3) {
        Particle.publish("weightChange", String(globalWeight), PRIVATE);
        previousWeight = globalWeight;
        consecutiveCount = 0;
    }
}