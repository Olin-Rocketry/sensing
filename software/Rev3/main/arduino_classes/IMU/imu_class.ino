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
  imu::Quaternion quat = bno.getQuat();
  
//  // Display the quat data
//  Serial.print("qW: ");
//  Serial.print(quat.w(), 4);
//  Serial.print(" qX: ");
//  Serial.print(quat.y(), 4);
//  Serial.print(" qY: ");
//  Serial.print(quat.x(), 4);
//  Serial.print(" qZ: ");
//  Serial.print(quat.z(), 4);
//  Serial.println("");
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

imu::Vector<3> accel Imu::read_accelerometer() {
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
//  print_data(accel);
//  Serial.print(data.x());
//    Serial.print(",");
//
//  Serial.print(data.y());
//  Serial.print(",");
//  Serial.println(data.z());
}

void Imu::read_linear_accel() {
  imu::Vector<3> linear_accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  print_data(linear_accel);
}

void Imu::print_data(imu::Vector<3> data) {
  /* Display the floating point data */
//  Serial.print("X: ");
//  Serial.print(data.x());
//  Serial.print(" Y: ");
//  Serial.print(data.y());
//  Serial.print(" Z: ");
//  Serial.print(data.z());
//  Serial.println("");  
Serial.print(data.x());
    Serial.print(",");

  Serial.print(data.y());
  Serial.print(",");
  Serial.println(data.z());
}
