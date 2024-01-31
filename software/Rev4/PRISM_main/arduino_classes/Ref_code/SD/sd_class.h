#pragma once
#include <Arduino.h>

#include <SD.h>
#include <SPI.h>



class Sd{
  private:
    int chipSelect;
    File dataFile;
    
    
  public:
    Sd(int chipSelect);
    void init();
    void begin();
    void write_sd(String dataString);
    
};
