#ifndef MAGNETIC_H
#define MAGNETIC_H

#include <Arduino.h>

const int mag_pin = A0;

void setup_magnetic();
int get_magnetic_baseline();
int get_magnetic_direction(int baseline);
int get_magnetic_raw();
float get_magnetic_voltage();

#endif