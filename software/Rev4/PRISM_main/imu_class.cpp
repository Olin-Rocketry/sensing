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

void Imu::setup_sensor()
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
    setup_sensor();
}

void Imu::perform_reading() {
  rotate();
  lsm.read();
}



Vector3 Imu::read_euler()
{
    sensors_event_t a, m, g, temp;
    lsm.getEvent(&a, &m, &g, &temp);
    Vector3 euler;
    euler.x = a.acceleration.x;
    euler.y = a.acceleration.y;
    euler.z = a.acceleration.z;
    return euler;

}

void Imu::read_gravity()
{
    sensors_event_t a, m, g, temp;
    lsm.getEvent(&a, &m, &g, &temp);
    Vector3 gravity;
    gravity.x = m.magnetic.x;
    gravity.y = m.magnetic.y;
    gravity.z = m.magnetic.z;
}

void Imu::read_gyroscope()
{
    sensors_event_t a, m, g, temp;
    lsm.getEvent(&a, &m, &g, &temp);
    Vector3 gyroscope;
    data->gyrox((float)g.gyro.x);
    data->gyroy((float)g.gyro.y);
    data->gyroz((float)g.gyro.z);

}

Vector3 Imu::read_accelerometer()
{
    sensors_event_t a, m, g, temp;
    lsm.getEvent(&a, &m, &g, &temp);
    Vector3 accel;
    accel.x = a.acceleration.x;
    accel.y = a.acceleration.y;
    accel.z = a.acceleration.z;
    return accel;
}

Vector3 Imu::read_linear_accel() // need to fix this
{
    sensors_event_t a, m, g, temp;
    lsm.getEvent(&a, &m, &g, &temp);
    Vector3 linear_accel;
    linear_accel.x = a.acceleration.x;
    linear_accel.y = a.acceleration.y;
    linear_accel.z = a.acceleration.z;
    return linear_accel;
}

Vector3 Imu::rotateVectorByQuaternion(const Vector3& vec, const Quaternion& quat) {
    // Quaternion multiplication
    double qx = quat.x;
    double qy = quat.y;
    double qz = quat.z;
    double qw = quat.w;
    
    double vx = vec.x;
    double vy = vec.y;
    double vz = vec.z;

    // Calculate the rotated vector using quaternion multiplication
    double x = (1 - 2 * (qy * qy + qz * qz)) * vx + 2 * (qx * qy - qw * qz) * vy + 2 * (qx * qz + qw * qy) * vz;
    double y = 2 * (qx * qy + qw * qz) * vx + (1 - 2 * (qx * qx + qz * qz)) * vy + 2 * (qy * qz - qw * qx) * vz;
    double z = 2 * (qx * qz - qw * qy) * vx + 2 * (qy * qz + qw * qx) * vy + (1 - 2 * (qx * qx + qy * qy)) * vz;

    return {(float)x, (float)y, (float)z};
}

void Imu::print_data(Vector3 data)
{
    /* Display the floating point data */
    Serial.print(data.x);
    Serial.print(",");
    Serial.print(data.y);
    Serial.print(",");
    Serial.println(data.z);
}


Quaternion Imu::ToQuaternion(double roll, double pitch, double yaw) // roll (x), pitch (y), yaw (z), angles are in radians
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


Quaternion Imu::read_quaternions()
{
    // Read Euler angles
    Vector3 eulers = read_euler();

    // Convert Euler angles to quaternions
    return ToQuaternion(eulers.x, eulers.y, eulers.z);
}


void Imu::rotate()
{
    Vector3 accel = read_accelerometer();

    // rotate to global using chip quaternion
    Quaternion unit_quat = read_quaternions();
    double norm = sqrt(unit_quat.w * unit_quat.w + unit_quat.x * unit_quat.x + unit_quat.y * unit_quat.y + unit_quat.z * unit_quat.z);
    unit_quat.w /= norm;
    unit_quat.x /= norm;
    unit_quat.y /= norm;
    unit_quat.z /= norm;

    // rotate the acceleration
    Vector3 rotated_accel = rotateVectorByQuaternion(accel, unit_quat);

    // save acceleration to data
    data->accelx((float)rotated_accel.x);
    data->accely((float)rotated_accel.y);
    data->accelz((float)rotated_accel.z);

    // Convert quaternion to Euler angles and set them
    set(unit_quat);

    // save eulers
    Vector3 eulers = read_euler();
    data->eulerx((float)eulers.x);
    data->eulery((float)eulers.y);
    data->eulerz((float)eulers.z);
}
