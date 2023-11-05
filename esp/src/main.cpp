#include <Arduino.h>
#include <WiFi.h>


// put function declarations here:
void connectToWiFi(const char*, const char*);

const char* ssid     = "DESKTOP-O2OLJ1H 8056";  // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "L5w7393^";  // The password of the Wi-Fi network

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectToWiFi(ssid, password);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// put function definitions here:
void connectToWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
   Serial.println("Connecting");
   while(WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   Serial.println("");
   Serial.print("Connected to WiFi network with IP Address: ");
   Serial.println(WiFi.localIP());
}
