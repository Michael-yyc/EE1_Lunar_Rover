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
#include <infrared.h>

volatile unsigned int count; 
unsigned long start_time;

void IR_ISR() {
    count++;
}

int get_pulse_rate() {
    count = 0;
    int lambda;
    start_time = millis();

    attachInterrupt(digitalPinToInterrupt(IR_interrupt_pin), IR_ISR, RISING);
    while((millis() - start_time) < 500) {};
    detachInterrupt(digitalPinToInterrupt(IR_interrupt_pin));

    int pulse_rate = count * 2;

    Serial.print("Infrared pulse rate: ");
    Serial.print(pulse_rate);
    Serial.println(" Hz");

    if (pulse_rate != 0) {
        if (pulse_rate < 430) {
            lambda = 312;
        }
        else {
            lambda = 547;
        }
    }
    else {
        lambda = 0;
    }

    return lambda;
}