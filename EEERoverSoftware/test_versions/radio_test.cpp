/*
Age UART packets decoder

Circuit:
antenna -> amplifier -> envelope decoder -> comparator (push-pull)
serial pin 0 at comparator output node

Microcontroller input:
Binary low (0V) and high (3.3V) signal 
Represents ASCII character bits

Features:
- Utilises Metro M0's in-built UART decoding hardware
- RX pin = 0; TX pin = 1
*/


//test version - change Serial.read() to Serial1.read() after testing
#include <Arduino.h>

char age[4];

void setup() {
    Serial.begin(115200);
    Serial1.begin(600);
    age[3] = '\0';
}

void loop() {
    while (Serial1.available()) {
        char c = '\0';

        if (c != '#') {
            c = Serial1.read();

            if (c == '#') {
                while (Serial1.available() < 3) {}; //wait until 3 bytes have been decoded
                for (int i = 0; i < 3; i++) {
                    age[i] = Serial1.read();
                }
            }
            Serial.print("Rock age: ");
            Serial.println(age);
        }
    }
}