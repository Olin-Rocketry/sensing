#include "altimeter.h"
Data data;
Altimeter a(&data);

byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data

void setup(){
  Serial.begin(115200);
  while (!Serial);
  data.SDbegin();
  a.begin_altimeter();
}

void loop(){
  a.read_altitude();
  //a.read_temperature();
  a.read_pressure();
  Serial.println(data.baralt());
}
