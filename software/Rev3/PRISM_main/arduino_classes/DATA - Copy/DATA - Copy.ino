#include "data_class.h"


Data test;
float timestep=0.0;

void setup() {
  Serial.begin(9600); 
  while(!Serial){};
  Serial.println("Starting");
  test.beginSD();

}
void loop() {
//  printData(test);
//  Serial.println(timestep);
//  test.accelx(timestep);
//  test.curtime(timestep);
  //Serial.println(test.accelx());
//  test.encodeAndAdd();
//  test.writeSDData();
//  delay(1000);
//  timestep+=1;
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
