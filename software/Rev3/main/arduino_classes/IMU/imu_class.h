#pragma once
#include <Arduino.h>
#include <Adafruit_BNO055.h>


class Imu {
  
  private:
    byte pin;
    Adafruit_BNO055 bno;
    float x_accel_global;
    float y_accel_global;
    float z_accel_global;
    
  public:
    Imu();
    void init();
    void test_connection();
    imu::Quaternion read_quaternions();
    void read_euler();
    void read_gravity();
    void read_gyroscope();
    imu::Vector<3> read_accelerometer();
    void read_linear_accel();
    void rotate();
    void print_data(imu::Vector<3> data);
};
