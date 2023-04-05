#include "imu_class.h"
#include "data_class.h"
#include <SPI.h>
#include <SD.h>
#include <algorithm>
#include <Adafruit_BNO055.h>

// setup for testing
byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data

// declare instance of Imu as global for scope
Data data;

Imu test(data);

void setup()
{
    //  Wire.begin();
      Serial.begin(9600);
      data.SDbegin();

      test.begin_imu();
}

void loop()
{
    //  test.test_connection();
    //  test.read_accelerometer();
    test.rotate();
    Serial.println(data.accelx());
}
