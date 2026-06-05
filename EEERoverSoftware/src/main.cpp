#include <Arduino.h>
#include <web_server.h>
#include <magnetic.h>

int magnetic_baseline, direction;

HardwareSerial& UART_decoder = Serial1;

void setup() {
  Serial.begin(115200);
  UART_decoder.begin(600);
  magnetic_baseline = get_magnetic_baseline();
  setup_server();
}


void loop() {
  //Call the server polling function
  handle_server_client();
}