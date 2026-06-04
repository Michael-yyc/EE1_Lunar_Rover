#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>
#include <WiFiUdp.h>
#include <ArduinoMDNS.h>
#include <webpage.cpp>
#include <motor_control.cpp>
#include <magnetic.cpp>


const char ssid[] = "AndroidAP";
const char pass[] = "chizz12345678";
const int groupNumber = 0; //only use to set IP address for EEERover network
const char hostname[] = "ZAMBAM";


WiFiWebServer server(80);
WiFiUDP udp;
MDNS mdns(udp);


//Return the web page
void handleRoot() {
    server.send(200, F("text/html"), webpage);
}

//Switch LED on and acknowledge
void ledON() {
    digitalWrite(LED_BUILTIN,1);
    server.send(200, F("text/plain"), F("ON"));
}

//Switch LED on and acknowledge
void ledOFF() {
    digitalWrite(LED_BUILTIN,0);
    server.send(200, F("text/plain"), F("OFF"));
}


//Generate a 404 response with details of the failed request
void handleNotFound() {
    String message = F("File Not Found\n\n"); 
    message += F("URI: ");
    message += server.uri();
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += server.args();
    message += F("\n");

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, F("text/plain"), message);
}


void setup_server() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);

    //Check WiFi shield is present
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println(F("WiFi shield not present"));
        while (true);
    }

    //Configure the static IP address if group number is set (!= 0)
    if (groupNumber) {
        WiFi.config(IPAddress(192,168,0,groupNumber+1));
    }

    // attempt to connect to WiFi network
    Serial.print(F("Connecting to WPA SSID: "));
    Serial.println(ssid);

    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
        delay(500);
        Serial.print('.');
    }

    //Register the callbacks to respond to HTTP requests
    server.on(F("/"), handleRoot);
    server.on(F("/on"), ledON);
    server.on(F("/off"), ledOFF);
    server.on(F("/F"), forward);
    server.on(F("/B"), backward);
    server.on(F("/L"), left);
    server.on(F("/R"), right);
    server.on(F("/S"), stop);
    server.on(F("/magnetic"), magnetic_scan);
    server.onNotFound(handleNotFound);

    server.begin();
    mdns.begin(WiFi.localIP(), hostname); //fixes root url to http://ZAMBAM.local/

    Serial.println(F("HTTP server started @ "));
    Serial.print(F("http://"));
    Serial.print(hostname);
    Serial.println(F(".local"));

    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}


//Server polling function
void handle_server_client() {
    mdns.run();
    server.handleClient();
}