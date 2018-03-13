#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>

#define BUFSIZE 16

byte mac[] {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 192, 17);

EthernetServer server(80);

byte piPins[] = {3, 5, 8, 9};
byte resetState;

void(* resetFunc) (void) = 0;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  delay(1000); //give ethernet shield time to come out of reset state
   
  resetState = EEPROM.read(0);
  
  if (resetState == 0) {
    EEPROM.write(0, 1);    
    resetFunc();
  }

  EEPROM.write(0, 0);

  for (byte i = 0; i <= 3; i++) {
    pinMode(piPins[i], OUTPUT);
    digitalWrite(piPins[i], LOW);
  }

  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());

  digitalWrite(13, HIGH);
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    process(client);
  }
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
