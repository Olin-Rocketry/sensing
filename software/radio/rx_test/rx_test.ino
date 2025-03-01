#include "radio_class.h"

Radio radio;

void setup() {
  Serial.begin(115200);
  radio.begin();
}

void loop() {
  char* receivedPacket = radio.readRadio(); // Call function to get received data
  
  if (receivedPacket) { // Check if a packet was received
      Serial.print("Processed Packet: ");
      Serial.println(receivedPacket);

      // Free the dynamically allocated memory
      delete[] receivedPacket;
  }
    
  delay(1000); // Just a delay to simulate periodic checking
}
