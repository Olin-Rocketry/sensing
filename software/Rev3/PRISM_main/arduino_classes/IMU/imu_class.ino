#include "imu_class.h"

Imu::Imu() {
  this->bno = Adafruit_BNO055(55, 0x28);
  init();
}

void Imu::init() {
  Wire.begin();
  Serial.begin(9600);
  test_connection();
}

void Imu::test_connection() {
  // throw error if no connection
  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  bno.setExtCrystalUse(true);
  delay(1000);
}

imu::Quaternion Imu::read_quaternions() {
  return bno.getQuat();
}

void Imu::rotate() {
  imu::Vector<3> accel = read_linear_accel();

  // rotate to rocket orientation
  imu::Quaternion rocket_quat = imu::Quaternion(0.7071, 0, 0.7071, 0); // for 90 degree rotations about y axis
  accel = rocket_quat.rotateVector(accel);

  // rotate to global using chip quaternion
  imu::Quaternion unit_quat = bno.getQuat();
  unit_quat.normalize(); // have to normalize first

  // test the product of both rotations
  imu::Quaternion product_quat = rocket_quat * unit_quat;
  print_data(product_quat.toEuler());

  // rotate the acceleration
  imu::Vector<3> rotated_accel = unit_quat.rotateVector(accel);
//  print_data(rotated_accel);
}


void Imu::read_euler() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  print_data(euler); 
}

void Imu::read_gravity() {
  imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  print_data(gravity);
}

void Imu::read_gyroscope() {
  imu::Vector<3> gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  print_data(gyroscope);
}

imu::Vector<3> Imu::read_accelerometer() {
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  return accel;
}

imu::Vector<3> Imu::read_linear_accel() {
  imu::Vector<3> linear_accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  return linear_accel;
}

void Imu::print_data(imu::Vector<3> data) {
  /* Display the floating point data */
  Serial.print(data.x());
  Serial.print(",");
  Serial.print(data.y());
  Serial.print(",");
  Serial.println(data.z());
}
