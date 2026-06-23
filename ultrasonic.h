#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

const int ultrasonic_pin = A1;

void setup_ultrasonic();
bool get_ultrasonic_detected();
float get_ultrasonic_voltage();

#endif