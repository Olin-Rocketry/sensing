#pragma once
#include "data_class.h"
#include <Arduino.h>
#include <Adafruit_BNO055.h>

class Imu
{

private:
  Data *data;
  Adafruit_BNO055 bno;

public:
  Imu(Data *data);
  void init();
  void begin_imu();
  void test_connection();
  imu::Quaternion read_quaternions();

  /* Rotate the acceleration into the global refernce frame.
   *
   * Get the linear acceleration. Get the quaternion of the chip.
   * Rotate the acceleration by the defined quaternion for the rocket
   * orinetation, then rotate by the chip quaternion to transfer into
   * global refernce frame.
   */
  void rotate();

  imu::Vector<3> read_euler(); // from the chip
  void read_gravity();
  void read_gyroscope();
  imu::Vector<3> read_accelerometer();
  imu::Vector<3> read_linear_accel();
  void print_data(imu::Vector<3> data);
};
