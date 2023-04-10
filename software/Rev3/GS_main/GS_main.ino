#include "radio_class.h"

Radio radio;

void setup()
{
  Serial8.begin(115200);
  Serial.begin(115200);
  radio.begin();
  while (!Serial.available())
    ;
}

void loop()
{
  radio.receivedPacket();
}
