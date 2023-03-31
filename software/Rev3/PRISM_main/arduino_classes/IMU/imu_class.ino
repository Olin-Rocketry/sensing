#include "imu_class.h"

Imu::Imu() {
  this->bno = Adafruit_BNO055(55, 0x28);
//  this->global_accel = NULL;
//  this->unit_quaternion = NULL;
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

imu::Quaternion Imu::normalize_quaternion() {
  imu::Quaternion quat = read_quaternions();
  float norm = sqrt(pow(quat.w, 2) + pow(quat.x, 2) + pow(quat.y, 2) + pow(quat.z, 2));
  return imu::Quaternion quat(quat.w / norm, quat.x / norm, quat.y / norm, quat.z / norm);
}



void Imu::rotate() {
  imu::Vector<3> accel = read_acceleration();
  imu::Quaternion unit_quat = normalize_quaternion();
  imu::Vector<3> quat_vector{unit_quat.x, unit_quat.y, unit_quate.z};
  float scalar = quat.w;
  imu::Vector<3> rotated_accel = 2.0f * dot(quat_vect, accel) * quat_vect
                            + (scalar * scalar - dot(quat_vect, quat_vect) * accel
                            + 2.0f * scalar * cross(quat_vect, scalar);
  print_data(rotated_accel);
}

float Imu::dot(imu::Vector<3> vect1, imu::Vector<3> vect2) {
    return (vect1.x * vect2.x) + (vect1.t * vect2.y) + (vect3.z * vect3.z);
}

imu::Vector<3> Imu::cross(imu::Vector<3> vect1, imu::Vector<3> vect2) {
  return imu::Vector<3> vect((vect1.y * vect2.z - vect1.z * vect2.y), -(vect1.x * vect2.z - vect1.z * vect2.x), (vect1.x * vect2.y - vect1.y * vect2.x));
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
