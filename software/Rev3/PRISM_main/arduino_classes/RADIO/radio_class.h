#pragma once
#include <Arduino.h>
#include <algorithm>
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 915.0


class Radio {
  private:
    RH_RF95 rf95;
    const static int dataPointCount = 27;
    float flightData[dataPointCount];
    char packet[dataPointCount*4];
    union floatunion_t;
    float decoder(char* encoded);

  public:
    Radio(); 
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
