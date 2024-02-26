#include "data_class.h"
#include <SD.h>


Data test;
float timestep=0.0;

void setup() {
  Serial.begin(9600); 
  Serial.println("Starting");
  test.SDbegin();
}
void loop() {
//  printData(test);
  Serial.print("timestep: ");
  Serial.println(timestep);
  test.accelx(timestep);
  test.accely(timestep*timestep);
  test.accelz(timestep/3);
  test.curtime((float)millis());
  //Serial.println(test.accelx());
  test.encodeAndAdd();
  delay(1);
  timestep+=1;
}

void printData(Data test)
{
  for(int i=0; i<5; i++)
  {
    Serial.print(test.flightData[i]);
    Serial.print(":");
    Serial.print(test.encodedFlightData[4*i]);
    Serial.print(test.encodedFlightData[4*i+1]);
    Serial.print(test.encodedFlightData[4*i+2]);
    Serial.println(test.encodedFlightData[4*i+3]);
  }
}
