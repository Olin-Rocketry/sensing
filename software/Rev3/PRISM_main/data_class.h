
#pragma once
#include "Arduino.h"
#include "led_class.h"
 #include <SD.h>
 #include <SerialTransfer.h>

 #include <algorithm>
#ifdef __AVR__
#define FsFile File32
#endif
class Data
{
private:
  union floatunion_t;
  void bulkencode(float *in, char *out);
  void encoder(char *encoded, float input);
  Led *statusLed;

  bool debugEnable; //Enable debug printing
  
  struct __attribute__((packed)) STRUCT {
    float lat;
    float lng;
    float gpsalt;
  } gpsStruct;
  File dataFile;
  int teensyLED = 13;

  String header="Time   \tAccel X   \tAccel Y   \tAccel Z   \tGyro X   \tGyro Y   \tGyro Z   \tTemp   \tEuler X   \tEuler Y   \tEuler Z   \tBarometric Alt   \tLongitude   \tLatitude   \tGPS Alt   \tPhase   \tContinuity   \tVoltage   \tLink Strength   \tKF x   \tKF y   \tKF z   \tKF Vx   \tKF Vy   \tKF Vz   \tKF drag   \tN/A   \t";

public:

  const static int packetSize = 27; //each packet is made up of N floats 27 ------------------------------------------
  const static int frameSize = 500; //each frame is made up of N packets
  int frameIndex = 0; //current location in the frame
  float packet[packetSize];  //current un-encoded packet
  char encodedpacket[packetSize * 4]; //current encoded packet
  char encodedFrame[frameSize][packetSize * 4];  //current encoded frame
  char fileName[17] = "flightLog000.txt";  
  SerialTransfer PRISM_serial;   //serial communication objects

  

  Data(Led *statusLed);
  void init();
  void SDbegin(bool debugEnable);
  void encodeAndAdd();
  void encodepacket();
  void addToFrame();
  void writeSDData();
  void sendSerialData();
  void readGPS();
  void analogTelem();
  void dprint(String in);

  float accelx();
  float accely();
  float accelz();
  float gyrox();
  float gyroy();
  float gyroz();
  float temp();
  float eulerx();
  float eulery();
  float eulerz();
  float baralt();
  float d();
  float lng();
  float lat();
  float gpsalt();
  float phs();
  float cont();
  float volt();
  float link();
  float kfx();
  float kfy();
  float kfz();
  float kfvx();
  float kfvy();
  float kfvz();
  float kfdrag();
  float curtime();

  
  void accelx(float i);
  void accely(float i);
  void accelz(float i);
  void gyrox(float i);
  void gyroy(float i);
  void gyroz(float i);
  void temp(float i);
  void eulerx(float i);
  void eulery(float i);
  void eulerz(float i);
  void baralt(float i);
  void d(float i);
  void lng(float i);
  void lat(float i);
  void gpsalt(float i);
  void phs(float i);
  void cont(float i);
  void volt(float i);
  void link(float i);
  void kfx(float i);
  void kfy(float i);
  void kfz(float i);
  void kfvx(float i);
  void kfvy(float i);
  void kfvz(float i);
  void kfdrag(float i);
  void curtime(float i);
};
