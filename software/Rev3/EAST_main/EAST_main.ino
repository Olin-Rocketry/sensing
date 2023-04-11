#include "gps_class.h"
#include "radio_class.h"

Gps gps;
Radio radio;

void setup()
{
  Serial8.begin(115200);
  gps.begin_gps();
  radio.begin();
}

void loop()
{
  gps.read_position();

  // radio
  radio.sendingPacket();
}
