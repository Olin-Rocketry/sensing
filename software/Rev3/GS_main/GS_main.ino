 cv               v v c hn#include "radio_class.h"

Radio radio;

void setup()
{

  Serial.begin(115200);
  radio.begin();
  while(!Serial){};
  delay(100);
  Serial.write(64);
  
//  while (!Serial.available());
}

void loop()
{
  radio.receivedPacket();
}
