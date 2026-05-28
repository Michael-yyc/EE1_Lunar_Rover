#include <web_server.cpp>
#include <magnetic.cpp>

int magnetic_baseline;

void setup() {
  Serial.begin(9600);

  setup_server();
  magnetic_baseline = get_magnetic_baseline();
}


void loop() {
  //Call the server polling function
  handle_server_client();

  //process hall sensor data
  magnetic_scan(magnetic_baseline);
}