#include "gps_class.h"
#include "radio_class.h"


Led statusLed(22);
Radio radio;
Gps gps(&radio);

unsigned long cycle_time;
unsigned long old_cycle_time = 0;

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
  cycle_time = millis() - old_cycle_time;
  
  if(cycle_time > 1000){
    
     gps.read_position();
     old_cycle_time = millis();
     
  }
 
  // radio

  radio.sendingPacket();


}
