#ifndef INFRARED_H
#define INFRARED_H

#include <Arduino.h>

const int IR_interrupt_pin = 3;

void setup_infrared();
void update_infrared();

int get_infrared_rate();
int get_infrared_lambda();
bool get_infrared_detected();

#endif