#include "gps_class.h"
#include "radio_class.h"

Gps gps;
Radio radio;

// **TO RUN**: Need to go into arduino library for sparkfun i2c gps 
// and change all instances of `MAX_PACKET_SIZE` 
// to `MAX_PACKET_SIZE_GPS`

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
