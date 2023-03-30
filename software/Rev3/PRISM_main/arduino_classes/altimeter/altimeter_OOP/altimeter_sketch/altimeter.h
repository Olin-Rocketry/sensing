#pragma once
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#define SEALEVELPRESSURE_HPA (1013.25)


class Altimeter {
  
  private:
    Adafruit_BMP3XX bmp;
    
  public:
    Altimeter();// not used
    void init();
    void readAlt();
    void readPres();
    void readTemp();

};
