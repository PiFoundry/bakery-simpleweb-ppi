#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>


// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;

byte piPins[] = {8, 9, 10, 11};

void setup() {
  for (byte i = 0; i< 4; i++) {
    pinMode(piPins[i], OUTPUT);
  }

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    process(client);
    client.stop();
  }

  delay(50); // Poll every 50ms
}

void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');  
  if (command == "poweron") {
    powerCommand(client, true);    
    
  }

  if (command == "poweroff") {
    powerCommand(client, false);    
  }

}

void powerCommand(BridgeClient client, bool on) {
  int pi = client.parseInt();
  SerialUSB.println(pi);
  digitalWrite(piPins[pi], on);
  client.print("ok");
}
