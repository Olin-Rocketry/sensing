#include "imu_class.h"

Imu::Imu(Data *data)
{
    this->data = data;
    init();
}

void Imu::init()
{
    test_connection();
}

void Imu::setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G, lsm.LSM9DS1_ACCELDATARATE_10HZ);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
}

void Imu::begin_imu(bool debugEnable)
{
    Wire.begin();
    this->debugEnable=debugEnable;
    if (debugEnable == true) {
      Serial.println("Imu started");
    }    
}

void Imu::test_connection()
{
    // throw error if no connection
    if (!lsm.begin())
    {
        Serial.print("Ooops, no LSM9DS1 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    Serial.println("LSM9DS1 detected");
    delay(1000);
    setupSensor();
}

imu::Quaternion Imu::read_quaternions()
{
    // Read Euler angles
    imu::Vector<3> eulers = read_euler();

    // Convert Euler angles to quaternions
    return ToQuaternion(eulers.x(), eulers.y(), eulers.z());
}

void Imu::perform_reading() {
  rotate();
  lsm.read();
}

struct Quaternion
{
    double w, x, y, z;
};

Quaternion ToQuaternion(double roll, double pitch, double yaw) // roll (x), pitch (y), yaw (z), angles are in radians
{

    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);

    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

void Imu::rotate()
{
    imu::Vector<3> accel = read_accelerometer();

    // rotate to global using chip quaternion
    Quaternion unit_quat = read_quaternions();
    double norm = sqrt(unit_quat.w * unit_quat.w + unit_quat.x * unit_quat.x + unit_quat.y * unit_quat.y + unit_quat.z * unit_quat.z);
    unit_quat.w /= norm;
    unit_quat.x /= norm;
    unit_quat.y /= norm;
    unit_quat.z /= norm;

    // rotate the acceleration
    imu::Vector<3> rotated_accel = unit_quat.rotateVector(accel);

    // save acceleration to data
    data->accelx((float)rotated_accel.x());
    data->accely((float)rotated_accel.y());
    data->accelz((float)rotated_accel.z());

    // Convert quaternion to Euler angles and set them
    set(unit_quat);

    // save eulers
    imu::Vector<3> eulers = read_euler();
    data->eulerx((float)eulers.x());
    data->eulery((float)eulers.y());
    data->eulerz((float)eulers.z());
}

void Imu::set(Quaternion q1) {
    double test = q1.x*q1.y + q1.z*q1.w;
    if (test > 0.499) { // singularity at north pole
        heading = 2 * atan2(q1.x,q1.w);
        attitude = PI/2;
        bank = 0;
        return;
    }
    if (test < -0.499) { // singularity at south pole
        heading = -2 * atan2(q1.x,q1.w);
        attitude = -PI/2;
        bank = 0;
        return;
    }
    double sqx = q1.x*q1.x;
    double sqy = q1.y*q1.y;
    double sqz = q1.z*q1.z;
    heading = atan2(2*q1.y*q1.w-2*q1.x*q1.z , 1 - 2*sqy - 2*sqz);
    attitude = asin(2*test);
    bank = atan2(2*q1.x*q1.w-2*q1.y*q1.z , 1 - 2*sqx - 2*sqz);
}

imu::Vector<3> Imu::read_euler()
{
    sensors_event_t a;
    lsm.getEvent(NULL, NULL, &a);
    imu::Vector<3> euler;
    euler.x = a.acceleration.x;
    euler.y = a.acceleration.y;
    euler.z = a.acceleration.z;
    return euler;

}

imu::Vector<3> read_gravity()
{
    sensors_event_t m;
    lsm.getEvent(NULL, &m, NULL);
    imu::Vector<3> gravity;
    gravity.x = m.magnetic.x;
    gravity.y = m.magnetic.y;
    gravity.z = m.magnetic.z;
    return gravity;

}

imu::Vector<3> read_gyroscope()
{
    sensors_event_t g;
    lsm.getEvent(&g, NULL, NULL);
    imu::Vector<3> gyroscope;
    gyroscope.x = g.gyro.x;
    gyroscope.y = g.gyro.y;
    gyroscope.z = g.gyro.z;
    return gyroscope;
}

imu::Vector<3> Imu::read_accelerometer()
{
    sensors_event_t a;
    lsm.getEvent(&a, NULL, NULL);
    imu::Vector<3> accel;
    accel.x = a.acceleration.x;
    accel.y = a.acceleration.y;
    accel.z = a.acceleration.z;
    return accel;
}

imu::Vector<3> Imu::read_linear_accel()
{
    sensors_event_t linear_a;
    lsm.getEvent(NULL, NULL, &linear_a);
    imu::Vector<3> linear_accel;
    linear_accel.x = linear_a.acceleration.x;
    linear_accel.y = linear_a.acceleration.y;
    linear_accel.z = linear_a.acceleration.z;
    return linear_accel;
}

void Imu::print_data(imu::Vector<3> data)
{
    /* Display the floating point data */
    Serial.print(data.x());
    Serial.print(",");
    Serial.print(data.y());
    Serial.print(",");
    Serial.println(data.z());
}
