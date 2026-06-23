#include <Arduino.h>
#include "infrared.h"

volatile unsigned int count = 0;

int infrared_rate = 0;
int infrared_lambda = 0;
bool infrared_detected = false;

void IR_ISR() {
    count++;
}

void setup_infrared() {
    pinMode(IR_interrupt_pin, INPUT);
    Serial.println("Infrared reader started");
}

void update_infrared() {
    count = 0;

    attachInterrupt(digitalPinToInterrupt(IR_interrupt_pin), IR_ISR, RISING);

    unsigned long start_time = millis();

    while ((millis() - start_time) < 500) {
        // Count pulses for 500 ms
    }

    detachInterrupt(digitalPinToInterrupt(IR_interrupt_pin));

    infrared_rate = count * 2;

    if (infrared_rate < 0) {
        infrared_rate = 0;
    }

    if (infrared_rate > 250 && infrared_rate < 430) {
        infrared_lambda = 312;
        infrared_detected = true;
    }
    else if (infrared_rate >= 430) {
        infrared_lambda = 547;
        infrared_detected = true;
    }
    else {
        infrared_lambda = 0;
        infrared_detected = false;
    }

    Serial.print("Infrared pulse rate: ");
    Serial.print(infrared_rate);
    Serial.print(" Hz | category -> ~");
    Serial.println(infrared_lambda);
}

int get_infrared_rate() {
    return infrared_rate;
}

int get_infrared_lambda() {
    return infrared_lambda;
}

bool get_infrared_detected() {
    return infrared_detected;
}
