#pragma once
#include "data_class.h"
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include <TinyGPS++.h>
#include <SerialTransfer.h>
 


class Gps {
  
  private:
    Data *data;
//    byte pin;
    I2CGPS i2c_gps;
    TinyGPSPlus gps;
    SerialTransfer mySend;
    
  public:
    Gps(Data *data);
    void init();
    void begin_gps();
    void test_connection();
    void read_position();
    void read_data();
};
