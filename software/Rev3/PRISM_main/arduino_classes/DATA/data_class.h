
#pragma once
#include <Arduino.h>
#include <SD.h>
//#include "SdFat.h"
#include <SPI.h>
#include <algorithm>
#ifdef __AVR__
#define FsFile File32
#endif
class Data {
  private:
    
    
    union floatunion_t;
    void bulkencode(float* in, char* out);
    void encoder(char* encoded, float input);
    
  public:
    const static int dataPointCount=28; //27 ------------------------------------------
    const static int batchSize=50;
    int batchCounter=0;
    float flightData[dataPointCount];
    char encodedFlightData[dataPointCount*4];
    char encodedBatch[batchSize][dataPointCount*4];
    char filename[10];
  
    Data();
    void init();
    float accelx();
    float accely();
    float accelz();
    float gyrox();
    float gyroy();
    float gyroz();
    float temp();
    float a();
    float b();
    float c();
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
    void a(float i);
    void b(float i);
    void c(float i);
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

    void begin();
    void encodeAndAdd();
    void encodeFlightData();
    void addToBatch();
    void writeSDData();
};
