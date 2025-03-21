#pragma once
#include <Arduino.h>
#include <algorithm>
#include <RH_RF95.h>
#include <SerialTransfer.h>

#define RFM95_CS 0
#define RFM95_RST 20
#define RFM95_INT 21
#define RF95_FREQ 915.0

class Radio {
  private:
    RH_RF95 rf95;
    const static int packetSize = 28;
    float packet[packetSize];
    char encodedPacket[packetSize*4];
    union floatunion_t;
    float decoder(char* encoded);
    SerialTransfer GS_serial;
    char buf[packetSize*4];

  public:
    Radio() : rf95(RFM95_CS,RFM95_INT) { }
    void init();  
    void begin(); 
    void receivedPacket();  
    void readRadio(); 
    void sendRadio(char* buffer2);
    void decodeData();  
    void printData();
    void sendingPacket();
    char* readSerial();
};
