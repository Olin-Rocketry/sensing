#include "imu_class.h"

//setup for testing 
byte IICdata[5] = {0,0,0,0,0}; //buffer for sensor data

// declare instance of Imu as global for scope
Imu test();


void setup() {
//  Wire.begin();
//  Serial.begin(9600);
}

void loop() {
//  test.test_connection();
//  test.read_accelerometer(); 
test.rotate();
}
