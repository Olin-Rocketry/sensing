#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include <TinyGPS++.h>
#include <SerialTransfer.h>
#include "led_class.h"
#include "radio_class.h"

class Gps
{

private:
  I2CGPS i2c_gps;
  TinyGPSPlus gps;
  SerialTransfer EAST_serial;
  struct __attribute__((packed)) STRUCT
  {
    float lat;
    float lng;
    float gpsalt;
  } gpsStruct;

  Radio *radio;
  Led *statusLed;

public:
  Gps(Radio *radio);
  void init();
  void begin_gps(Led *statusLed);
  void test_connection();
  void read_position();
  void read_data();
};
