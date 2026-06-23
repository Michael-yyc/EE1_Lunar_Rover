#include <SPI.h>
#include <WiFi101.h>

#include "webpage.h"
#include "radio.h"
#include "magnetic.h"
#include "infrared.h"
#include "ultrasonic.h"
#include "rock_classifier.h"

char ssid[] = "Rover_Dashboard";
char pass[] = "password123";

WiFiServer server(80);

/* Global Variables */
String latest_age = "";
bool age_detected = false;

int magnetic_baseline = 0;
int magnetic_direction = 0;

int ir_rate = 0;
int ir_lambda = 0;
bool ir_detected = false;

bool ultrasonic_detected = false;

String rock_type = "UNKNOWN";

// =====================
// Send webpage
// =====================
void sendPage(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  const char* p = webpage;
  char buffer[64];
  int i = 0;

  while (*p) {
    buffer[i++] = *p++;

    if (i == 63) {
      buffer[i] = '\0';
      client.print(buffer);
      i = 0;
      delay(1);
    }
  }

  if (i > 0) {
    buffer[i] = '\0';
    client.print(buffer);
  }
}

// =====================
// Send JSON data
// =====================
void sendData(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  client.println("{");

  // Age section
  client.println("\"age\":{");

client.print("\"detected\":");
client.print(age_detected ? "true" : "false");
client.println(",");

client.print("\"value\":\"");
if (latest_age == "") {
  client.print("---");
} else {
  client.print(latest_age);
}
client.println("\"");

client.println("},");

  // Rock section
  client.println("\"rock\":{");

  client.println("\"id\":1,");

  client.print("\"type\":\"");
  client.print(rock_type);
  client.println("\",");

  client.println("\"confidence\":100,");

  // IR
  client.print("\"ir\":{\"detected\":");
  client.print(ir_detected ? "true" : "false");
  client.print(",\"value\":\"");
  if (ir_lambda == 0) {
    client.print("NONE");
  } else {
    client.print(ir_lambda);
    client.print(" s^-1");
  }
  client.println("\"},");

  // Ultrasound
  client.print("\"ultrasound\":{\"detected\":");
  client.print(ultrasonic_detected ? "true" : "false");
  client.print(",\"value\":\"");
  client.print(ultrasonic_detected ? "40 kHz" : "NONE");
  client.println("\"},");

  // Magnetic
  client.print("\"magnetic\":{\"detected\":");
  client.print(magnetic_direction != 0 ? "true" : "false");
  client.print(",\"value\":\"");
  client.print(magnetic_to_text(magnetic_direction));
  client.println("\"}");

  client.println("}");

  client.println("}");
}

// =====================
// Handle website client
// =====================
void handleClient() {
  WiFiClient client = server.available();

  if (client) {
    String requestLine = "";

    unsigned long timeout = millis();

    while (client.connected() && millis() - timeout < 1000) {
      if (client.available()) {
        requestLine = client.readStringUntil('\r');
        Serial.println(requestLine);

        if (requestLine.indexOf("GET /data") >= 0) {
          sendData(client);
        }
        else {
          sendPage(client);
        }

        break;
      }
    }

    delay(1);
    client.stop();
  }
}

// =====================
// Setup
// =====================
void setup() {
  Serial.begin(115200);

  unsigned long start = millis();
  while (!Serial && millis() - start < 3000);

  Serial.println("Starting rover system...");

  analogReadResolution(12);

  // Sensor setup
  setup_radio(Serial1);
  setup_infrared();
  setup_ultrasonic();
  setup_magnetic();

  // Calibrate magnetic sensor with no magnet/rock present
  magnetic_baseline = get_magnetic_baseline();

  Serial.print("Magnetic baseline: ");
  Serial.println(magnetic_baseline);

  // Wi-Fi shield check
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  Serial.println("Creating access point...");
  WiFi.beginAP(ssid, pass);

  delay(5000);

  IPAddress ip = WiFi.localIP();

  Serial.print("AP IP address: ");
  Serial.print(ip[0]);
  Serial.print(".");
  Serial.print(ip[1]);
  Serial.print(".");
  Serial.print(ip[2]);
  Serial.print(".");
  Serial.println(ip[3]);

  server.begin();
  Serial.println("Server started");
}

// =====================
// Loop
// =====================
void loop() {
  // Age
  String new_age = get_age();

if (new_age != "") {
  latest_age = new_age;
  age_detected = true;
}

  // Infrared
  update_infrared();
  ir_rate = get_infrared_rate();
  ir_lambda = get_infrared_lambda();
  ir_detected = get_infrared_detected();

  // Ultrasound
  ultrasonic_detected = get_ultrasonic_detected();

  // Magnetic
  magnetic_direction = get_magnetic_direction(magnetic_baseline);

  // Rock classification
  rock_type = classify_rock(ir_lambda, ultrasonic_detected, magnetic_direction);

  // Website
  handleClient();
}