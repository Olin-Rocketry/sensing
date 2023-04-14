#pragma once
#include "led_class.h"
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
    const static int packetSize = 27;
    float packet[packetSize];
    char encodedPacket[packetSize*4];
    union floatunion_t;
    float decoder(char* encoded);
    char serialBuffer[packetSize*4];
    Led *statusLed;

    
    

  public:
  
//    Radio(Led *statusLed);
    Radio() : rf95(RFM95_CS,RFM95_INT) { }  //what is going on here? We need to make this a constructor method 

    SerialTransfer EAST_serial;

    
    void init();  
    void begin(); 
    void reveicePacket();  
 
    void sendRadio(char serialBuffer[packetSize*4]);
//void sendRadio();
void readRadio();
    void decodeData();  
    void printData();
    void sendingPacket();
    char readSerial();
};
