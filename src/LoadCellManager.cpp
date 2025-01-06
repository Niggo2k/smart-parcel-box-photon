#include "LoadCellManager.h"
#include "Particle.h"

double globalWeight = 0.0;
#define HX711_DT D5
#define HX711_CLK D6

HX711ADC LoadCell(HX711_DT, HX711_CLK);

void setupLoadCell() {
    LoadCell.begin();
    LoadCell.tare();
    LoadCell.set_scale(12.70f);
    Particle.variable("weight", globalWeight);
    Particle.function("GetWeight", GetWeight);
    Particle.function("CalibrateScale", calibrateScale);
    Particle.function("TareWeight", TareWeight);
}

int GetWeight(String command) {
    float weight = LoadCell.get_units(10);
    float weightKg = weight / 1000.0;
    globalWeight = round(weightKg * 10) / 10.0;
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
    Serial.println("Calibration complete.");
    return 1;
}