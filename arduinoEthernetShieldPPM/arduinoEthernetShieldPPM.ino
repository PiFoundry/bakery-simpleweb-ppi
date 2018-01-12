#include <SPI.h>
#include <Ethernet.h>

#define BUFSIZE 16

byte mac[] {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192,168,192,17);

EthernetServer server(80);

byte piPins[] = {2, 3, 4, 5, 6, 7};

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  for (byte i = 0; i<= 5; i++) {
    pinMode(piPins[i], OUTPUT);
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
  
  while (client.connected()) {
    if (client.available()) {
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
            client.println("HTTP/1.1 200 OK");
          } else if (strcmp(command, "poweroff") == 0) {
            digitalWrite(piPins[id], LOW);                              
            client.println("HTTP/1.1 200 OK");
          }
        }
        
        client.println("HTTP/1.1 404 NotFound");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("ok");        
        
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
