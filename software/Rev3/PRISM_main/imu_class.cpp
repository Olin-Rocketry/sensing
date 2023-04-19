#include "imu_class.h"

Imu::Imu(Data *data)
{
    this->bno = Adafruit_BNO055(55, 0x28);
    this->data = data;
    init();
}

void Imu::init()
{

    test_connection();
}

void Imu::begin_imu()
{
    Wire.begin();
}

void Imu::test_connection()
{
    // throw error if no connection
    if (!bno.begin())
    {
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    bno.setExtCrystalUse(true);
    delay(1000);
}

imu::Quaternion Imu::read_quaternions()
{
    return bno.getQuat();
}

void Imu::rotate()
{
    imu::Vector<3> accel = read_linear_accel();

//    // rotate to rocket orientation
//    imu::Quaternion rocket_quat = imu::Quaternion(0.7071, 0, 0.7071, 0); // for 90 degree rotations about y axis
//    accel = rocket_quat.rotateVector(accel);

    // rotate to global using chip quaternion
    imu::Quaternion unit_quat = bno.getQuat();
    unit_quat.normalize(); // have to normalize first

    // test the product of both rotations
//    imu::Quaternion product_quat = rocket_quat * unit_quat;
//    imu::Vector<3> eulers = product_quat.toEuler();

//    // store the euler angles into data
//    data->eulerx((float)eulers.x());
//    data->eulery((float)eulers.y());
//    data->eulerz((float)eulers.z());

    // rotate the acceleration
    imu::Vector<3> rotated_accel = unit_quat.rotateVector(accel);
//      print_data(rotated_accel);

    // save values to data
    data->accelx((float)rotated_accel.x());
    data->accely((float)rotated_accel.y());
    data->accelz((float)rotated_accel.z());

    // save eulers
    imu::Vector<3> eulers = read_euler();
    data->eulerx((float)eulers.x());
    data->eulery((float)eulers.y());
    data->eulerz((float)eulers.z());
}

imu::Vector<3> Imu::read_euler()
{
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return euler;
}

void Imu::read_gravity()
{
    imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
//    print_data(gravity);
}

void Imu::read_gyroscope()
{
    imu::Vector<3> gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    data->gyrox((float)gyroscope.x());
    data->gyroy((float)gyroscope.y());
    data->gyroz((float)gyroscope.z());
    //  print_data(gyroscope);
}

imu::Vector<3> Imu::read_accelerometer()
{
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    return accel;
}

imu::Vector<3> Imu::read_linear_accel()
{
    imu::Vector<3> linear_accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
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
