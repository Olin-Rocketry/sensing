#pragma once
#include "data_class.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_AHRS_Madgwick.h"

struct Quaternion // Define Quaternion struct here
    {
        double w, x, y, z;
    };

struct Vector3 {
    float x;
    float y;
    float z;
};


class Imu
{
private:
    Data *data;
    Adafruit_LSM9DS1 lsm;
    Adafruit_Madgwick madgwick9dof; // Initialize Madgwick Orientation Filter object
    double dt;
    double old_time = 0;
    double new_time;
    double heading;
    double attitude;
    double bank;


    bool debugEnable; // Enable debug printing

public:
    Imu(Data *data);
    void init();
    void setup_sensor();
    void begin_imu(bool debugEnable);
    void test_connection();
    Quaternion read_quaternions();
    void perform_reading();
    void read_and_fuse();
    /* Rotate the acceleration into the global refernce frame.
   *
   * Get the linear acceleration. Get the quaternion of the chip.
   * Rotate the acceleration by the defined quaternion for the rocket
   * orientation.
   */
    void rotate(); // call rotate() and read_gyroscope()
    Vector3 read_euler();        // from the chip
    Vector3 read_magnetometer();
    Vector3 read_gyroscope();
    Vector3 read_accelerometer();
    Vector3 read_linear_accel();
    void print_data(Vector3 data);
    Quaternion ToQuaternion(double roll, double pitch, double yaw); // Quaternion calculation
    Vector3 rotateVectorByQuaternion(const Vector3& vec, const Quaternion& quat);
    void set(Quaternion q1); // Set Euler angles from Quaternion
};
