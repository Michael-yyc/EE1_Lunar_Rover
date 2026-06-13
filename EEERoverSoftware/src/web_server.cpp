#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>
#include <WiFiUdp.h>
#include <ArduinoMDNS.h>
#include <web_server.h>
#include <webpage.h>
#include <motor_control.h>
#include <radio.h>
#include <magnetic.h>
#include <infrared.h>
#include <ultrasound.h>

const char ssid[] = "RoverAP";
const char pass[] = "password123";
const int groupNumber = 0; //only use to set IP address for EEERover network
const char hostname[] = "ZAMBAM";


WiFiWebServer server(80);
WiFiUDP udp;
MDNS mdns(udp);

//Return the web page
void handleRoot() {
    server.send(200, F("text/html"), plan_z);
}

//Switch LED on and acknowledge
void ledON() {
    digitalWrite(LED_BUILTIN,1);
    server.send(200, F("text/plain"), F("ON"));
}

//Switch LED off and acknowledge
void ledOFF() {
    digitalWrite(LED_BUILTIN,0);
    server.send(200, F("text/plain"), F("OFF"));
}

//
void STATE() {
    int raw = analogRead(A0);
    float voltage = raw * (3.3/1023);
    const int threshold = 1.65;
    if (voltage < threshold) {
        server.send(200, F("text/plain"), F("OFF"));
    }
    else {
        server.send(200, F("text/plain"), F("ON"));
    }
}

void age_scan() {
    int age = get_age();
    server.send(200, F("text/plain"), String(age));
}

void magnetic_scan() {
    int field = get_magnetic_direction();

    if (field == 1) {
        server.send(200, F("text/plain"), F("UP"));
    }
    else if (field == -1) {
        server.send(200, F("text/plain"), F("DOWN"));
    }
    else {
        server.send(200, F("text/plain"), F("NO FIELD DETECTED"));
    }
}

void IR_scan() {
    int lambda = get_pulse_rate();

    if (lambda != 0) {
        server.send(200, F("text/plain"), String(lambda));
    }
    else {
        server.send(200, F("text/plain"), F("N/A"));
    }
}

void US_scan() {
    bool US_present = get_US_status();
    
    if (US_present) {
        server.send(200, F("text/plain"), F("DETECTED"));
    }
    else {
        server.send(200, F("text/plain"), F("NOT DETECTED"));
    }
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
    server.on(F("/state"), STATE);
    server.on(F("/on"), ledON);
    server.on(F("/off"), ledOFF);
    server.on(F("/F"), forward);
    server.on(F("/B"), backward);
    server.on(F("/L"), left);
    server.on(F("/R"), right);
    server.on(F("/S"), stop);
    server.on(F("/age"), age_scan);
    server.on(F("/magnetic"), magnetic_scan);
    server.on(F("/IR"), IR_scan);
    server.on(F("/US"), US_scan);
    server.onNotFound(handleNotFound);

    server.begin();
    mdns.begin(WiFi.localIP(), hostname); //fixes root url to http://ZAMBAM.local/

    Serial.println(F("HTTP server started @ "));
    Serial.print(F("http://"));
    Serial.print(hostname);
    Serial.println(F(".local"));

    Serial.print(F("IP address: "));
    Serial.print(F("http://"));
    Serial.println(static_cast<IPAddress>(WiFi.localIP()));
    Serial.println("(^if the host network name doesn't work, use this IP address)");
}


//Server polling function
void handle_server_client() {
    mdns.run();
    server.handleClient();
}