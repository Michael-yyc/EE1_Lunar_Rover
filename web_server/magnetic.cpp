#include <Arduino.h>
#include "magnetic.h"

const int num_samples = 100;
const int threshold = 125;

int latest_magnetic_raw = 0;
float latest_magnetic_voltage = 0.0;

// Average 100 readings to reduce noise
int average_reading() {
    long total = 0;

    for (int i = 0; i < num_samples; i++) {
        total += analogRead(mag_pin);
        delay(1);
    }

    return total / num_samples;
}

void setup_magnetic() {
    analogReadResolution(12);
    pinMode(mag_pin, INPUT);
}

int get_magnetic_baseline() {
    int baseline = average_reading();

    double baseline_voltage = (baseline * 3.3) / 4095.0;

    Serial.print("Baseline Voltage: ");
    Serial.println(baseline_voltage, 3);

    return baseline;
}

int get_magnetic_direction(int baseline) {
    latest_magnetic_raw = average_reading();

    int deviation = latest_magnetic_raw - baseline;

    latest_magnetic_voltage = (latest_magnetic_raw * 3.3) / 4095.0;

    Serial.print("Magnetic ADC: ");
    Serial.print(latest_magnetic_raw);

    Serial.print(" | Voltage: ");
    Serial.print(latest_magnetic_voltage, 3);

    Serial.print(" V | Deviation: ");
    Serial.print(deviation);

    if (deviation > threshold) {
        Serial.println(" | DOWN");
        return -1;   // DOWN
    }
    else if (deviation < -threshold) {
        Serial.println(" | UP");
        return 1;    // UP
    }
    else {
        Serial.println(" | NONE");
        return 0;    // no field
    }
}

int get_magnetic_raw() {
    return latest_magnetic_raw;
}

float get_magnetic_voltage() {
    return latest_magnetic_voltage;
}