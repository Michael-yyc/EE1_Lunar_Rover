#include <Arduino.h>
#include "radio.h"

HardwareSerial* UART_decoder = nullptr;

char age_digits[4];

void setup_radio(HardwareSerial& uart) {
    UART_decoder = &uart;
    UART_decoder->begin(600);

    age_digits[3] = '\0';

    Serial.println("Radio age reader started");
}

String get_age() {
    if (UART_decoder == nullptr) {
        return "";
    }

    if (UART_decoder->available()) {
        char c = UART_decoder->read();

        Serial.print("RX: ");
        Serial.println(c);

        if (c == '#') {
            Serial.println("Start marker detected");

            unsigned long timeout = millis();

            while (UART_decoder->available() < 3) {
                if (millis() - timeout > 200) {
                    Serial.println("Timeout waiting for age digits");
                    return "";
                }
            }

            for (int i = 0; i < 3; i++) {
                age_digits[i] = UART_decoder->read();
            }

            Serial.print("Raw age digits: ");
            Serial.println(age_digits);

            String age = "";
            age += age_digits[0];
            age += ".";
            age += age_digits[1];
            age += age_digits[2];

            Serial.print("Decoded age: ");
            Serial.print(age);
            Serial.println(" billion years");

            return age;
        }
    }

    return "";
}