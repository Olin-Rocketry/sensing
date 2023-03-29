#include "altimeter.h"
Altimeter a;
void setup(){
  Serial.begin(115200);
  while (!Serial);
};

void loop(){
  a.readAlt();
  delay(2000);
  a.readTemp();
  delay(2000);
  a.readPres();
  delay(2000);
};
