#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Rover_AP";
const char* password = "password123";

WebServer server(80);

void handleRoot() {
    server.send(200, "text/html", "<h1>ESP32 Rover Server Working</h1>");
}

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssid, password);

    Serial.println("ESP32 Access Point started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.begin();

    Serial.println("Web server started");
}

void loop() {
    server.handleClient();
}