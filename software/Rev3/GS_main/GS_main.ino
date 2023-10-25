#include "radio_class.h"

Radio radio;
bool connected;
void connect_GS();


void setup()
{

  Serial.begin(115200);
  radio.begin();
  while(!Serial){};
  connect_GS();
  

  
//  while (!Serial.available());
}

void loop()
{

  if (connected){
    radio.receivedPacket();
  }
  
  else{
    connect_GS();
}

  while(!Serial){
    connected = false;
  }
  
  
}


void connect_GS(){
  Serial.write(64);
  connected = true;
}
