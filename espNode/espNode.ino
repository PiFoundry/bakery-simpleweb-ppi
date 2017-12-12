#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define piId "687680cd" //put your piId here

const char* ssid = ""; //put your WLAN SSID Here
const char* password = ""; //Put your WLAN Password here

//don't touch anything below
bool state, stateChanged;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600, SERIAL_8N1, SERIAL_TX_ONLY);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);   
  }

  MDNS.begin(piId);
  Serial.print("IP Address:");
  Serial.print(WiFi.localIP());
  
  state = false;
  stateChanged = true;

  server.on("/poweroff", handlePowerOff);
  server.on("/poweron", handlePowerOn);
  server.begin();
}

void loop() {
  if (stateChanged) {
    stateChanged = false;
    relayOn(state);          
  }

  server.handleClient();

}

void relayOn(bool s) {
  if (s) {
    Serial.write(0xA0);
    Serial.write(0x01);
    Serial.write(0x01);
    Serial.write(0xA2);
  } else {
    Serial.write(0xA0);
    Serial.write(0x01);
    Serial.write(0x00);
    Serial.write(0xA1);
  }
}

void handlePowerOff() {  
  if (server.argName(0) == "id" && server.arg(0) == "0") {     
     state = false;
     stateChanged = true;
     server.send(200, "text/plain", "ok");      
  } else {  
    server.send(400, "text/plain", "no id given");   
  }
}

void handlePowerOn() {  
  if (server.argName(0) == "id" && server.arg(0) == "0") {    
     state = true;
     stateChanged = true;
     server.send(200, "text/plain", "ok");      
  } else {  
    server.send(400, "text/plain", "no id given");   
  }
}
