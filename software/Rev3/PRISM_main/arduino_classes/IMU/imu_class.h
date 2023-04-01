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
    imu::Quaternion read_quaternions();

    /* Rotate the acceleration into the global refernce frame.
     *  
     * Get the linear acceleration. Get the quaternion of the chip.
     * Rotate the acceleration by the defined quaternion for the rocket
     * orinetation, then rotate by the chip quaternion to transfer into
     * global refernce frame.
    */
    void rotate();
    
    void read_euler();
    void read_gravity();
    void read_gyroscope();
    imu::Vector<3> read_accelerometer();
    imu::Vector<3> read_linear_accel();
    void print_data(imu::Vector<3> data);
};
