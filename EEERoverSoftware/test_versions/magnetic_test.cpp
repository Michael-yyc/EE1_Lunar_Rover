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

const int mag_pin = A0;
const int num_samples = 100; // averaging amount
const int threshold = 125; // sensitivity threshold (ADC counts) -> ~ 0.1V
int baseline;


// function to average 100 readings
int average_reading() {
    long total = 0;

    for (int i = 0; i < num_samples; i++) {
        total += analogRead(mag_pin);
        delay(1);
    }

    return total / num_samples;
}


void setup() {
    analogReadResolution(12);
    pinMode(mag_pin, INPUT);

    Serial.begin(115200);
    while(!Serial) {};

    baseline = average_reading();
    double baseline_voltage = (baseline * 3.3) / 4095.0;

    Serial.print("Baseline Voltage: ");
    Serial.println(baseline_voltage);
}


void loop() {
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
        Serial.println("   --> MAGNETIC FIELD: DOWN");
    }
    else if (deviation < -threshold) {
        Serial.println("   --> MAGNETIC FIELD: UP");
    }
    else {
        Serial.println("   --> NO FIELD DETECTED");
    }

    delay(1000);
}
