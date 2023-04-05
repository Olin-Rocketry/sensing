#include "altimeter.h"
Altimeter a;
void setup(){
  Serial.begin(115200);
  while (!Serial);
};

void loop(){
  a.read_altitude();
  delay(2000);
  a.read_temperature();
  delay(2000);
  a.read_pressure();
  delay(2000);
};
