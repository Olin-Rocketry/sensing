#include "radio_class.h"

Radio radio;

void setup() {
  Serial.begin(115200);
  radio.begin();
}

void loop() {
  char radiopacket[20] = "Hello World";
  char* packet = radiopacket;
  radio.sendRadio(packet);
  radio.readRadio();
  delay(1000);
}
