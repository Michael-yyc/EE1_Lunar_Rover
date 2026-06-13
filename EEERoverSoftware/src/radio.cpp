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


//test version - change Serial.read() to UART_decoder.read() after testing
#include <Arduino.h>
#include <radio.h>

extern HardwareSerial& UART_decoder;

int get_age() {
    static int last_age = 0;
    
    if (UART_decoder.available()) {
        char c = '\0';
        char age[4];

        age[3] = c;

        while (c != '#') {
            c = UART_decoder.read();

            if (c == '#') {
                while (UART_decoder.available() < 3) {}; //wait until 3 bytes have been decoded

                for (int i = 0; i < 3; i++) {
                    age[i] = UART_decoder.read();
                }
            }
        }

        Serial.print("Rock age: ");
        Serial.println(age);

        int age_int = atoi(age);
        last_age = atoi(age);
        return age_int;
    }
    else {
        return last_age;
    }
}