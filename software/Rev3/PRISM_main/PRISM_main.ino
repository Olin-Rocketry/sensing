#include "arduino_classes/LED/led_class.h"
#include "arduino_classes/Data/data_class.h"
#include "arduino_classes/IMU/imu_class.h"



//setup for testing 
byte IICdata[5] = {0,0,0,0,0}; //buffer for sensor data

Led led1(12);
// declare instance of Imu as global for scope
Data data;

Imu test(data);

void setup() {
//  Wire.begin();
//  Serial.begin(9600);
test.begin_imu();
}

void loop() {
//  test.test_connection();
//  test.read_accelerometer(); 
test.rotate();
Serial.println(data.accelx());
}
