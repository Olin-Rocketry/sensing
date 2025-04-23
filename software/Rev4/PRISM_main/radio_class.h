#pragma once
#include "led_class.h"
#include <Arduino.h>
#include <algorithm>
#include <RH_RF95.h>
#include <RH_RF69.h>
#include "data_class.h"

//#define RFM69 // Comment for RF95 Module

#ifdef RFM69
  #define RFM69_CS 9
  #define RFM69_RST 8
  #define RFM69_INT 7
  #define RF69_FREQ 434.0
#else
  #define RFM95_CS 9
  #define RFM95_RST 8
  #define RFM95_INT 7
  #define RF95_FREQ 915.0
#endif

class Radio {
  private:
    #ifdef RFM69
      RH_RF69 rf69;
    #else
      RH_RF95 rf95;
    #endif
    const static int packetSize = 28;
    float packet[packetSize];
    char encodedPacket[packetSize*4];
    union floatunion_t;
    float decoder(char* encoded);
    char serialBuffer[packetSize*4];
    Led *statusLed;
    bool serial_status;
    Data *data;

    
    

  public:

//    Radio(Led *statusLed);
    #ifdef RFM69
      Radio(Data *data) : rf69(RFM69_CS, RFM69_INT) {this->data = data; init();} ;
    #else
      Radio(Data *data) : rf95(RFM95_CS,RFM95_INT) {this->data = data; init();} ;
    #endif

    
    void init();  
    void begin(); 
    void reveicePacket();  
    void led_test(Led *statusLed);
 
    void sendRadio();
//void sendRadio();
void readRadio();
    void decodeData();  
    void printData();
    void sendingPacket();
    char readSerial();

};
