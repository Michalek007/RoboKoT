#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>

#define UART2_TXD 17 // UART2 TxD -> GPIO17
#define UART2_RXD 16 // UART2 RxD -> GPIO16

// put function declarations here:
void connectToWiFi(const char*, const char*);
void getAction(int& result);

const char* ssid     = "DESKTOP-O2OLJ1H 8056";  // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "L5w7393^";  // The password of the Wi-Fi network
WiFiClient client;
HTTPClient http;
String servername = "http://192.168.137.1:5000";
String urlGetAction = servername + "/get_action/";
HardwareSerial SerialPort(2);
int result{0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialPort.begin(28800, SERIAL_8N1, UART2_RXD, UART2_TXD);
  connectToWiFi(ssid, password);
}

void loop() { 
  getAction(result);
  if (result != 0)
  SerialPort.print(result);
  // if (SerialPort.available()){
  //   char output = SerialPort.read();
  //   Serial.println(output);
  // }
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

void getAction(int& result){
  http.begin(client, urlGetAction.c_str());
  int httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    result = (int) payload[10] - 48;
  }
}
