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

const int mag_pin = A0;
const int num_samples = 100; // averaging amount
const int threshold = 125; // sensitivity threshold (ADC counts) -> ~ 0.1V

// function to average 100 readings
int average_reading() {
    long total = 0;

    for (int i = 0; i < num_samples; i++) {
        total += analogRead(mag_pin);
    }

    return total / num_samples;
}


int get_magnetic_baseline() {
    analogReadResolution(12);
    pinMode(mag_pin, INPUT);

    Serial.begin(115200);
    while(!Serial) {};

    return average_reading();
}


int get_magnetic_direction(int baseline) {
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
        Serial.print("   --> MAGNETIC FIELD: DOWN");
        return -1;
    }
    else if (deviation < -threshold) {
        Serial.print("   --> MAGNETIC FIELD: UP");
        return 1;
    }
    else {
        Serial.print("   --> NO FIELD DETECTED");
        return 0;
    }
}
