#pragma once
#include "data_class.h"
#include <Arduino.h>
#include <Adafruit_BNO055.h>

class Imu
{

private:
  Data *data;
  Adafruit_BNO055 bno;
  
  double dt;
  double old_time=0;
  double new_time;
  
  bool debugEnable; //Enable debug printing


public:
  Imu(Data *data);
  void init();
  void begin_imu(bool debugEnable);
  void test_connection();
  imu::Quaternion read_quaternions();
  double integrated_velocity = 0;

  
  void perform_reading(); // call rotate() and read_gyroscope()
  /* Rotate the acceleration into the global refernce frame.
   *
   * Get the linear acceleration. Get the quaternion of the chip.
   * Rotate the acceleration by the defined quaternion for the rocket
   * orientation.
   */
  void rotate();
  imu::Vector<3> read_euler(); // from the chip
  void read_gravity();
  void read_gyroscope();
  imu::Vector<3> read_accelerometer();
  imu::Vector<3> read_linear_accel();
  void print_data(imu::Vector<3> data);
};
