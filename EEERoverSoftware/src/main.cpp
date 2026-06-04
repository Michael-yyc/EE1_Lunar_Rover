#include <web_server.cpp>
#include <magnetic.cpp>

int magnetic_baseline, direction;

void setup() {
  Serial.begin(115200);

  setup_server();
  magnetic_baseline = get_magnetic_baseline();
}


void loop() {
  //Call the server polling function
  handle_server_client();
}