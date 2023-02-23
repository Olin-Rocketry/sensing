#include <SD.h>
#include <SPI.h>
#include "Write_SD.h"
#include <Adafruit_MPL3115A2.h>
//Creating instances
Adafruit_MPL3115A2 baro;
Write_SD Set_SD = Write_SD();

void setup() {
  seperate_setup();
  
  

  
  
}

void loop() {
  
  float altitude = baro.getAltitude();

  Set_SD.SD_writing(altitude); 
}
