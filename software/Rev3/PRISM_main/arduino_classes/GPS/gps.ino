#include "gps_class.h"

//setup for testing 

// declare instance of Imu as global for scope
Gps gps;


void setup() {
//  Wire.begin();
//  Serial.begin(9600);
}

void loop() {
//  test.test_connection();
  gps.read_position(); 
}
