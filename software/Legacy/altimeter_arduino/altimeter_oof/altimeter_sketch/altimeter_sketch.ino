
#include "altimeter.h"
Altimeter a;
void setup(){
  Serial.begin(115200);
  while (!Serial);
};

void loop(){
  a.readAltitude();
};
