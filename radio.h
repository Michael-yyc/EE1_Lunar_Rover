#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>

void setup_radio(HardwareSerial& uart);
String get_age();

#endif