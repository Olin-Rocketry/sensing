#pragma once
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#define SEALEVELPRESSURE_HPA (1013.25)


class Altimeter {
  
  private:
    Data data;
    Adafruit_BMP3XX bmp;
    
  public:
    Altimeter(Data data);// not used
    void init();
    void read_altimeter();
    void read_pressure();
    void read_temperature();
};
