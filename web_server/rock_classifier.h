#ifndef ROCK_CLASSIFIER_H
#define ROCK_CLASSIFIER_H

#include <Arduino.h>

String classify_rock(int ir_lambda, bool ultrasonic_detected, int magnetic_direction);
String magnetic_to_text(int magnetic_direction);

#endif