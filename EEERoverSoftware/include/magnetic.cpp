/*
A1324 Hall Sensor Magnetic Direction Detection

Circuit:
Hall OUT -> 10k -> 20k -> GND
A0 pin at node between 10k and 20k

Hall sensor powered from 5V
Metro reads reduced 0–3.3V signal

Features:
- 12-bit ADC resolution
- Averaging to reduce noise
- Automatic baseline calibration
*/

#include <Arduino.h>

const int hallPin = A0;
const int numSamples = 100; // averaging amount
const int threshold = 375; // sensitivity threshold (ADC counts) -> ~ 0.3V


// function to average 100 readings
int average_reading() {
    long total = 0;

    for (int i = 0; i < numSamples; i++) {
        total += analogRead(hallPin);
        delay(1);
    }

    return total / numSamples;
}


int get_magnetic_baseline() {
    //12-bit ADC resolution
    analogReadResolution(12);

    int baseline = average_reading();

    return baseline;
}


int magnetic_scan(int baseline) {
    int reading = average_reading();

    int deviation = reading - baseline;

    float voltage = (reading * 3.3) / 4095.0; //converts binary ADC count to voltage between 0 - 3.3V

    Serial.print("ADC: ");
    Serial.print(reading);

    Serial.print("  Voltage: ");
    Serial.print(voltage, 3); //printed to 3 d.p

    Serial.print("  Deviation: ");
    Serial.print(deviation);

    //detect magnetic direction
    if (deviation > threshold) {
        return 1;
        Serial.print("   --> MAGNETIC FIELD: UP");
    }
    else if (deviation < -threshold) {
        return -1;
        Serial.print("   --> MAGNETIC FIELD: DOWN");
    }
    else {
        return 0;
        Serial.print("   --> NO FIELD DETECTED");
    }

    Serial.println();
}