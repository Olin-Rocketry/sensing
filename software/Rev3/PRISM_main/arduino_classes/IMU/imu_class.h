#pragma once
#include <Arduino.h>
#include <Adafruit_BNO055.h>


class Imu {
  
  private:
    byte pin;
    Adafruit_BNO055 bno;
    
  public:
    Imu();
    void init();
    void test_connection();
    void read_quaternions();
    void read_euler();
    void read_gravity();
    void read_gyroscope();
    void read_accelerometer();
    void read_linear_accel();
    void print_data(imu::Vector<3> data);
};
