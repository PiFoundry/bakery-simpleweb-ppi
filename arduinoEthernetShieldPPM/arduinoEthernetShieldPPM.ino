#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>

#define BUFSIZE 16

#define resetPin 2
#define checkRetries 1

byte mac[] {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 192, 17);

EthernetServer server(80);

byte piPins[] = {3, 5, 8, 9};
byte resetState;
IPAddress checkServer(192, 168, 192, 1);
int checkPort = 80;
unsigned long checkInterval = 3000;

void setup() {
  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT);
    
  for (byte i = 0; i <= 3; i++) {
    pinMode(piPins[i], OUTPUT);
    digitalWrite(piPins[i], LOW);
  }

  Serial.begin(9600);

  delay(1500); //give ethernet shield time to come out of reset state and for my crappy PSU to stabelize

  Ethernet.begin(mac, ip);
  
  delay(1000); //give ethernet shield time to initialize
  
  server.begin();
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());

}

void loop() {  
  static unsigned long lastCheck = 0;
  static byte retries = checkRetries;
  
  int checkResult;

  EthernetClient client = server.available();  
  if (client) {
    Serial.println("client connected");
    process(client);
  }
      
/*  if (millis() - lastCheck >= checkInterval) {
    Serial.println("checking if I'm connected");

    EthernetClient checkClient;
    checkResult = checkClient.connect(checkServer, checkPort);
    lastCheck = millis();
    
    if (checkResult != 1) {
      Serial.println("Health check failed");
      if (retries == 0) {
        Serial.println("Resetting.");
        Serial.flush();
        digitalWrite(resetPin, LOW);
      }
      retries--;
    } else {
      retries = checkRetries;           
    }
  }*/
}

void process(EthernetClient client) {
  boolean currentLineIsBlank = true;
  byte index = 0;
  char clientline[BUFSIZE];
  boolean success = false;

  while (client.connected()) {
    while (client.available()) {
      char c = client.read();

      if (index < BUFSIZE) {
        clientline[index++] = c;
      }

      if (c == '\n' && currentLineIsBlank) {
        client.flush();

        char *method = strtok(clientline, "/");
        char *command = strtok(NULL, "/");
        char *idstring = strtok(NULL, " ");
        byte id = atoi(idstring);

        Serial.print(command);
        Serial.print(": ");
        Serial.println(id);

        if (command != NULL && strcmp(method, "GET\n") && id >= 0 && id <= 5) {
          if (strcmp(command, "poweron") == 0 ) {
            digitalWrite(piPins[id], HIGH);
            success = true;
          } else if (strcmp(command, "poweroff") == 0) {
            digitalWrite(piPins[id], LOW);
            success = true;
          }
        }

        if (success) {
          client.println("HTTP/1.1 200 OK");
        } else {
          client.println("HTTP/1.1 404 Not Found");
        }
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.print("ok");

        delay(1);
        client.stop();
        break;
      }
      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
  }
}
