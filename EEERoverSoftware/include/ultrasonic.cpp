/*
40kHz Ultrasonic signal detection

Circuitry:
transducer OUT -> amplifier -> envelope detector -> comparator (push-pull)
serial pin 2 at comparator output node

Microcontroller input:
Either a binary low (0V) or a high (3.3V) signal
High indicates the presence of the 40kHz ultrasonic signal

Features:
- Guard against comparator output glitches
*/

//test version
#include <Arduino.h>

const int US_pin = 2;
bool US_present;

bool check_presence() {
    int count = 0;
    for (int i = 0; i < 5; i++) {
        count += digitalRead(US_pin);
        delay(1);
    }

    if (count < 3) {
        return false;
    }

    return true;
}

void setup() {
    Serial.begin(115200);
    pinMode(US_pin, INPUT);
}

void loop() {
    US_present = check_presence();
    if (US_present) {
        Serial.println("40kHz Ultrasonic Signal detected");
    }
    else {
        Serial.println("No 40kHz Ultrasonic Signal detected");
    }

    delay(1000);
}