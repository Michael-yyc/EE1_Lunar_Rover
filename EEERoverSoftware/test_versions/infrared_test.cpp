/*
Infrared Pulse Rate Calculator

Circuitry:
phototransistor output -> amplifier -> HP 2nd order filter -> comparator (push-pull)
serial pin 3 at comparator output node

Microcontroller input:
Binary low (0V) and high (3.3V) signal
High indicates an infrared pulse

Features:
- Interrupt Service Routine to continuously count pulses
- Automatic categorisation of pulse rates
*/

//test version
#include <Arduino.h>

const int IR_interrupt_pin = 3;
volatile unsigned int count = 0; 
unsigned long start_time;

void IR_ISR() {
    count++;
}

int get_pulse_rate() {
    count = 0;
    start_time = millis();

    attachInterrupt(digitalPinToInterrupt(IR_interrupt_pin), IR_ISR, RISING);
    while((millis() - start_time) < 500) {};
    detachInterrupt(digitalPinToInterrupt(IR_interrupt_pin));

    return count * 2;
}

void setup() {
    Serial.begin(115200);
    pinMode(IR_interrupt_pin, INPUT);
}

void loop() {
    int lambda;
    int pulse_rate = get_pulse_rate();

    Serial.print("Infrared pulse rate: ");
    Serial.print(pulse_rate);
    Serial.println(" Hz");

    if (pulse_rate < 430) {
        lambda = 312;
    }
    else {
        lambda = 547;
    }

    Serial.print("category -> ~");
    Serial.println(lambda);

    delay(1000);
}