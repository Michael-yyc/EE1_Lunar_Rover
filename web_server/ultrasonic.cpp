#include <Arduino.h>
#include "ultrasonic.h"

const float VREF = 3.3;
const int ADC_MAX = 4095;
const float ULTRASONIC_THRESHOLD = 1.0;

float latest_ultrasonic_voltage = 0.0;

void setup_ultrasonic() {
    pinMode(ultrasonic_pin, INPUT);
}

float read_ultrasonic_voltage() {
    long total = 0;

    for (int i = 0; i < 20; i++) {
        total += analogRead(ultrasonic_pin);
        delay(1);
    }

    float average_adc = total / 20.0;
    latest_ultrasonic_voltage = average_adc * VREF / ADC_MAX;

    return latest_ultrasonic_voltage;
}

bool get_ultrasonic_detected() {
    float voltage = read_ultrasonic_voltage();

    Serial.print("Ultrasonic voltage: ");
    Serial.print(voltage, 3);
    Serial.print(" V | ");

    if (voltage > ULTRASONIC_THRESHOLD) {
        Serial.println("40 kHz detected");
        return true;
    } else {
        Serial.println("No 40 kHz");
        return false;
    }
}

float get_ultrasonic_voltage() {
    return latest_ultrasonic_voltage;
}