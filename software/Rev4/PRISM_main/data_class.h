#pragma once
#include "Arduino.h"
#include "led_class.h"
#include <SD.h>
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
  bool noSD; // Enable no SD card
  
  struct __attribute__((packed)) STRUCT {
    float lat;
    float lng;
    float diagmsg;
    float gpsalt;
  } gpsStruct;
  File dataFile;
  int teensyLED = 13;

  String header="Time\t\t\tAccel X\t\tAccel Y\t\tAccel Z\t\tGyro X\t\tGyro Y\t\tGyro Z\t\tTemp\t\tEuler X\t\tEuler Y\t\tEuler Z\t\tBaro Alt\tLongitude\tLatitude\tGPS Alt\t\tPhase\t\tContinuity\tVoltage\t\tLink Strength\tKF x\t\tKF y\t\tKF z\t\tKF Vx\t\tKF Vy\t\tKF Vz\t\tKF drag\t\tN/A\t\tDiagnostic Message";

public:

  const static int packetSize = 28; //each packet is made up of N floats 28 ------------------------------------------
  const static int frameSize = 500; //each frame is made up of N packets
  int frameIndex = 0; //current location in the frame
  float packet[packetSize];  //current un-encoded packet
  char encodedpacket[packetSize * 4]; //current encoded packet
  char encodedFrame[frameSize][packetSize * 4];  //current encoded frame
  char fileName[18] = "flightLog0000.txt"; 
  bool validpacket = false; 

  

  Data(Led *statusLed);
  void init();
  void SDbegin(bool debugEnable, bool noSD);
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

  // defining new diagnostic messaging method
  float diagmsg();
  
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
  void diagmsg(float i);

  void diagmsg_reset();
};
