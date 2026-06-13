#include <Arduino.h>
#include <web_server.h>
#include <magnetic.h>
#include <infrared.h>
#include <ultrasound.h>

HardwareSerial& UART_decoder = Serial1;
const int mag_pin = A0;
const int IR_interrupt_pin = 2;
const int US_pin = 3;

void setup() {
  Serial.begin(115200);
  while (!Serial) {};
  UART_decoder.begin(600);

  pinMode(mag_pin, INPUT_PULLDOWN);
  get_magnetic_baseline();
  pinMode(IR_interrupt_pin, INPUT_PULLDOWN);
  pinMode(US_pin, INPUT_PULLDOWN);

  setup_server();
}


void loop() {
  //Call the server polling function
  handle_server_client();
}