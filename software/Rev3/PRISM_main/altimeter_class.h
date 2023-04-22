#pragma once
#include "data_class.h"
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#define SEALEVELPRESSURE_HPA (1013.25)

class Altimeter
{

  private:
    Data *data;
    Adafruit_BMP3XX bmp;
    float EMA_prev=-1;
    int Smoothing=2;
    int Sample=20;
    

  
    bool debugEnable; //Enable debug printing
  
  public:
    Altimeter(Data *data);
    void init();
    void begin_altimeter(bool debugEnable);
    void read_altitude();
    void read_pressure();
    void read_temperature();
    void perform_reading();
    void EMA();
    float EMA_value;
    double derived_velocity = 0;
    
};
