//#include "led_class.h"
#include "data_class.h"
#include "imu_class.h"

#include <Arduino.h>


// setup for testing
byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data

//Led led1(12);
// declare instance of Imu as global for scope


      
Data data;
Imu test(&data);


void setup()
{
    //  Wire.begin();

      Serial.begin(9600);
      
      data.SDbegin();

      test.begin_imu();
      test.test_connection();
}

void loop()
{
      
    //  test.read_accelerometer();
    test.rotate();
    test.read_gyroscope();
    data.encodeAndAdd();
    Serial.println(data.accelx());
}
//void loop()
//{
//      led1.red();
//      delay(1000);
//      led1.green();
//      delay(1000);
//      led1.off();
//      delay(1000);
//}
