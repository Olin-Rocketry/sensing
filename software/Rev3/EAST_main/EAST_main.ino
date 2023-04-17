#include "gps_class.h"
#include "radio_class.h"


Led statusLed(22);
Radio radio;
Gps gps(&radio);


void setup()
{

  Serial.begin(115200);
  radio.led_test(&statusLed);
  delay(10);
  
  
  radio.begin();
  gps.begin_gps(&statusLed);
  
  
}

void loop()
{
//  gps.read_position();
  // radio

  radio.sendingPacket();


}
