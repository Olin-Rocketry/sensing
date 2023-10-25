#include "radio_class.h"

Radio radio;
<<<<<<< Updated upstream
=======
bool connected;
void connect_GS();
>>>>>>> Stashed changes

void setup()
{

  Serial.begin(115200);
  radio.begin();
  while(!Serial){};
<<<<<<< Updated upstream
  delay(100);
  Serial.write(64);
=======
  connect_GS();
  
>>>>>>> Stashed changes
  
//  while (!Serial.available());
}

void loop()
{
<<<<<<< Updated upstream
  radio.receivedPacket();
=======
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
>>>>>>> Stashed changes
}
