#include "data_class.h"
#include <SD.h>


Data test;
float timestep=0.0;

void setup() {
  Serial.begin(9600); 
  Serial.println("Starting");
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    while (1) {
      // No SD card, so don't do anything more - stay stuck here
    }
  }
}
void loop() {
  printData(test);
  Serial.println(timestep);
//  test.accelx(timestep);
//  test.curtime(timestep);
  //Serial.println(test.accelx());
  test.encodeAndAdd();
  writeSDData(test);
  delay(1000);
  timestep+=1;
}

void printData(Data test)
{
  for(int i=0; i<27; i++)
  {
    Serial.print(test.flightData[i]);
    Serial.print(":");
    Serial.print(test.encodedFlightData[4*i]);
    Serial.print(test.encodedFlightData[4*i+1]);
    Serial.print(test.encodedFlightData[4*i+2]);
    Serial.println(test.encodedFlightData[4*i+3]);
  }
}

void writeSDData(Data test){
  SD.remove("testfile.txt");
  File dataFile = SD.open("testfile.txt", FILE_WRITE); // Not sure about this data type
  if (dataFile) {
      for (int batch=0; batch<test.batchCounter+1; batch++) //batchSize is the number of lines in a batch write //test.batchCounter
      {
//            char encoded[4*dataPointCount];
//            //dataPointCount is the number of unique numbers to send (27 at the time of writing this)
//            //since each is a float, you need four bytes/chars for each value in a batch
//            bulkencode(flightData[batch],encoded); //flightData is a two-dimensional array that is of size batchSize*dataPointCount
//            for (int index=0; index<4*dataPointCount; index++)
//            {
//              dataFile.print(encoded[index]);
//            }
        dataFile.print(test.encodedBatch[batch]);
        Serial.print("counter: ");
        Serial.println(test.batchCounter);
        Serial.print("batch: ");
        Serial.println(test.encodedBatch[batch]);
        dataFile.println();
      }
  }
  dataFile.close();
  test.batchCounter=0;
}
