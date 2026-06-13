/*
A1324 Hall Sensor Magnetic Direction Detection

Circuit:
hall sensor output -> 10k -> 20k -> GND
analogue pin 0 at node between 10k and 20k

Microcontroller input:
DC voltage between 0-3.3V

Features:
- Averaging to reduce noise
- Automatic baseline calibration
- Positive deviation -> downwards direction
*/

#include <Arduino.h>
#include <magnetic.h>

const int num_samples = 100; // averaging amount
const int threshold = 37; // sensitivity threshold (ADC counts) -> ~ 0.03V

int magnetic_baseline;

// function to average 100 readings
int average_reading() {
    long total = 0;

    for (int i = 0; i < num_samples; i++) {
        total += analogRead(mag_pin);
    }

    return total / num_samples;
}


void get_magnetic_baseline() {
    analogReadResolution(12);
    magnetic_baseline = average_reading();
}


int get_magnetic_direction() {
    int reading = average_reading();
    int deviation = reading - magnetic_baseline;

    float voltage = (reading * 3.3) / 4095.0; //converts binary ADC count to voltage between 0 - 3.3V
    Serial.print("Baseline: ");
    Serial.print(magnetic_baseline);
    Serial.print("   Voltage: ");
    Serial.println(voltage);

    //detect magnetic direction
    if (deviation > threshold) {
        return -1;
    }
    else if (deviation < -threshold) {
        return 1;
    }
    else {
        return 0;
    }
}
