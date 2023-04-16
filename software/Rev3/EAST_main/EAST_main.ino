#include "gps_class.h"
#include "radio_class.h"

//Gps gps;
Led statusLed(22);
//Radio radio(&statusLed);
Radio radio;



void setup()
{

  Serial.begin(115200);
  radio.led_test(&statusLed);
  
//  gps.begin_gps();
  radio.begin();
  
  
}

void loop()
{
  //gps.read_position();
  // radio

  radio.sendingPacket();
  delay(1);

}
