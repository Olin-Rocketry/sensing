#pragma once
#include <Arduino.h>
#include <algorithm>

//IMPORTANT: If you are using this library with an ESP32, make sure you are on the latest version and that you set it to use HSPI in the radiohead.h file
#include <RH_RF95.h>


#define RFM95_CS 26
#define RFM95_RST 27
#define RFM95_INT 25
#define RF95_FREQ 915.0

class Radio {
  private:
    RH_RF95 rf95;
    const static int packetSize = 28;
    float packet[packetSize];
    char encodedPacket[packetSize*4];
    union floatunion_t;
    float decoder(char* encoded);
    char serialBuffer[packetSize*4];
    bool serial_status;


    
    

  public:
  
    Radio() : rf95(RFM95_CS,RFM95_INT) { }  //what is going on here? We need to make this a constructor method 


    
    void init();  
    void begin(); 
    void receivePacket();  
    float getRSSI();
    void sendRadio(char serialBuffer[packetSize*4]);
//void sendRadio();
void readRadio();
    void decodeData();  
    void printData();
    void sendingPacket();
    char readSerial();

};
