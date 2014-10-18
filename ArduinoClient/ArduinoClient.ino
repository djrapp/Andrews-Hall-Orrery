#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Servo.h>
#include <DmxSimple.h>

#define LED 13
#define DMX_DIR 4
#define RED_CHANNEL 1
#define GRN_CHANNEL 2
#define BLU_CHANNEL 3

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } 


YunServer server;

int red=0, green=0, blue=0;

void setup() {
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(DMX_DIR, OUTPUT);  
  digitalWrite(DMX_DIR, HIGH);  // Set direction pin of trasnsceiver to Tx.
  DmxSimple.usePin(2);
}

void loop() {
  
  YunClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }
  
  Serial << "Red: " << red << "  Blue: " << blue << "  Green: " << green << "\n";
  
  delay(15);
  
}

void process(YunClient client) {
  String command = client.readStringUntil('/');

  // Check if the url contains the word "servo"
  if (command == "color") {
    extractColors(client);
    setColors();
  }
  
}

void extractColors(YunClient client) {
  red = client.parseInt();
  client.readStringUntil('/');
  green = client.parseInt();
  client.readStringUntil('/');
  blue = client.parseInt();
}

void setColors(){
    DmxSimple.write(RED_CHANNEL, red);
    DmxSimple.write(GRN_CHANNEL, green);
    DmxSimple.write(BLU_CHANNEL, blue);
}