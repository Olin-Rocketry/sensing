#pragma once
#include "data_class.h"
#include <Arduino.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include <TinyGPS++.h>
 


class Gps {
  
  private:
    byte pin;
    I2CGPS i2c_gps;
    TinyGPSPlus gps;
    
  public:
    Gps(Data data);
    void init();
    void test_connection();
    void read_position();
    void print_data();
};
