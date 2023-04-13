#include "radio_class.h"

Radio radio;

void setup()
{

  Serial.begin(115200);
  radio.begin();
  Serial.write(64);
//  while (!Serial.available());
}

void loop()
{
  radio.receivedPacket();
}
