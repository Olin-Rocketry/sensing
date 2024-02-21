#pragma once
#include "data_class.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h> // not used in this demo but required!

class Imu
{
private:
    Data *data;
    Adafruit_LSM9DS1 lsm;

    double dt;
    double old_time = 0;
    double new_time;

    bool debugEnable; // Enable debug printing

public:
    Imu(Data *data);
    void init();
    void setupSensor();
    void begin_imu(bool debugEnable);
    void test_connection();
    imu::Quaternion read_quaternions();
    /* Rotate the acceleration into the global reference frame.
   *
   * Get the linear acceleration. Get the quaternion of the chip.
   * Rotate the acceleration by the defined quaternion for the rocket
   * orientation.
   */    void perform_reading(); // call rotate() and read_gyroscope()
    void rotate();
    imu::Vector<3> read_euler();        // from the chip
    imu::Vector<3> read_gravity();
    imu::Vector<3> read_gyroscope();
    imu::Vector<3> read_accelerometer();
    imu::Vector<3> read_linear_accel();
    void print_data(imu::Vector<3> data);
    imu::Quaternion ToQuaternion(double roll, double pitch, double yaw); // Quaternion calculation
    void set(Quaternion q1); // Set Euler angles from Quaternion
};
