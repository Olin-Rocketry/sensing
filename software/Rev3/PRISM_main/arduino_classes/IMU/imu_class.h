#pragma once
#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <cmath>


class Imu {
  
  private:
    byte pin;
    Adafruit_BNO055 bno;
//    imu::Vector<3> global_accel;
//    imu::Quaternion unit_quaternion;
//    
  public:
    Imu();
    void init();
    void test_connection();
    imu::Quaternion read_quaternions();
    void normalize_quaternion();
    void rotate();
    float dot(imu::Vector<3> vect1, imu::Vector<3> vect2);
    imu::Vector<3> cross(imu::Vector<3> vect1, imu::Vector<3> vect2);
    void read_euler();
    void read_gravity();
    void read_gyroscope();
    void read_accelerometer();
    void read_linear_accel();
    void print_data(imu::Vector<3> data);
};
