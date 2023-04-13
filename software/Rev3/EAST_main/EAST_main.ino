#include "gps_class.h"
#include "radio_class.h"

//Gps gps;
Led statusLed(26);
//Radio radio(&statusLed);
Radio radio;



void setup()
{

  Serial.begin(115200);
//  gps.begin_gps();
  radio.begin();
}

void loop()
{
  //gps.read_position();
  // radio

  radio.sendingPacket();
  delay(5);

}
