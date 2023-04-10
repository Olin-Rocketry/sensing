#include "gps_class.h"
//#include <Arduino.h>
//#include <SD.h>



// declare instance of Imu as global for scope
Gps gps();


void setup() {
//  Wire.begin();
  Serial.begin(9600);
  gps.begin_gps();
//  gps.test_connection();
}

void loop() {
//  test.test_connection();
  gps.read_position(); 
}
