#ifndef CLASSES
#define CLASSES
#include <Arduino.h>
#include <SD.h>
#include <algorithm>
class Data {
  private:
    const static int dataPointCount;
    const static int batchSize;
    int batchCounter = 0;
    float flightData[dataPointCount];
    char encodedFlightData[dataPointCount*4];
    String encodedBatch[batchSize];
    
    union floatunion_t {
        float f;
        char a[sizeof (float) ];
    } float_u;
    
    void bulkencode(float* in, char* out) {
      //in and out should be char arrays of the same size
      for (int i=0; i<sizeof(in)/sizeof(in[0]); i++)
      {
        char encoded[sizeof (float) ];
        encoder(encoded, in[i]);
        encoder(encoded, in[i]);
        for(int j=0; j<sizeof(float); j++)
        {
          out[i*sizeof(float)+j]=encoded[j];
        }
      }
    }
    
    void encoder(char* encoded, float input)
    {
      float_u.f=input;
      for (int i = 0; i<sizeof (float); i++)
      {
        encoded[i] = float_u.a[i];
      }
    }
  public:
    Data(){
      std::fill_n(flightData, dataPointCount, 0.0);
      std::fill_n(encodedFlightData, dataPointCount*4, '0');
      std::fill_n(encodedBatch, batchSize, '0');
    }
  
    float accelx() {return flightData[0];}
    float accely() {return flightData[1];}
    float accelz() {return flightData[2];}
    float gyrox() {return flightData[3];}
    float gyroy() {return flightData[4];}
    float gyroz() {return flightData[5];}
    float temp() {return flightData[6];}
    float a() {return flightData[7];}
    float b() {return flightData[8];}
    float c() {return flightData[9];}
    float baralt() {return flightData[10];}
    float d() {return flightData[11];}
    float lng() {return flightData[12];}
    float lat() {return flightData[13];}
    float gpsalt() {return flightData[14];}
    float phs() {return flightData[15];}
    float cont() {return flightData[16];}
    float volt() {return flightData[17];}
    float link() {return flightData[18];}
    float kfx() {return flightData[19];}
    float kfy() {return flightData[20];}
    float kfz() {return flightData[21];}
    float kfvx() {return flightData[22];}
    float kfvy() {return flightData[23];}
    float kfvz() {return flightData[24];}
    float kfdrag() {return flightData[25];}
    float curtime() {return flightData[26];}
    void accelx(float i) {flightData[0]=i;}
    void accely(float i) {flightData[1]=i;}
    void accelz(float i) {flightData[2]=i;}
    void gyrox(float i) {flightData[3]=i;}
    void gyroy(float i) {flightData[4]=i;}
    void gyroz(float i) {flightData[5]=i;}
    void temp(float i) {flightData[6]=i;}
    void a(float i) {flightData[7]=i;}
    void b(float i) {flightData[8]=i;}
    void c(float i) {flightData[9]=i;}
    void baralt(float i) {flightData[10]=i;}
    void d(float i) {flightData[11]=i;}
    void lng(float i) {flightData[12]=i;}
    void lat(float i) {flightData[13]=i;}
    void gpsalt(float i) {flightData[14]=i;}
    void phs(float i) {flightData[15]=i;}
    void cont(float i) {flightData[16]=i;}
    void volt(float i) {flightData[17]=i;}
    void link(float i) {flightData[18]=i;}
    void kfx(float i) {flightData[19]=i;}
    void kfy(float i) {flightData[20]=i;}
    void kfz(float i) {flightData[21]=i;}
    void kfvx(float i) {flightData[22]=i;}
    void kfvy(float i) {flightData[23]=i;}
    void kfvz(float i) {flightData[24]=i;}
    void kfdrag(float i) {flightData[25]=i;}
    void curtime(float i) {flightData[26]=i;}

    void encodeAndAdd(){
      encodeFlightData();
      addToBatch();
    }
    
    void encodeFlightData(){
      bulkencode(flightData,encodedFlightData);
    }

    void addToBatch(){
      if (batchCounter>=batchSize){
        writeSDData();
        batchCounter=0;
      }
      encodedBatch[batchCounter]="";
      for (int index=0; index<4*dataPointCount; index++)
      {
        encodedBatch[batchCounter]+=encodedFlightData[index];
      }
      batchCounter++;
    }
    
    void writeSDData(){
      File dataFile = SD.open("flightData.txt", FILE_WRITE); // Not sure about this data type
      if (dataFile) {
          for (int batch=0; batch<batchCounter; batch++) //batchSize is the number of lines in a batch write
          {
//            char encoded[4*dataPointCount];
//            //dataPointCount is the number of unique numbers to send (27 at the time of writing this)
//            //since each is a float, you need four bytes/chars for each value in a batch
//            bulkencode(flightData[batch],encoded); //flightData is a two-dimensional array that is of size batchSize*dataPointCount
//            for (int index=0; index<4*dataPointCount; index++)
//            {
//              dataFile.print(encoded[index]);
//            }
            dataFile.print(encodedBatch[batch]);
            dataFile.println();
          }
      }
      dataFile.close();
    }
};
