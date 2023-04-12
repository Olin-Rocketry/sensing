// #include "led_class.h"
#include "data_class.h"
#include "imu_class.h"
#include "altimeter_class.h"


#include <Arduino.h>

// setup for testing
byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data

//Led led(26);
//  declare instance of Imu as global for scope

Data data;
Imu imu_test(&data);
Altimeter altimeter(&data);

void setup()
{
    Serial.begin(115200);

    data.SDbegin();
    imu_test.begin_imu();
    altimeter.begin_altimeter();
    //    test.test_connection();

//    led.RGB(0, 0, 0, 100);
//    led.RGB(1, 0, 0, 100);
}

void loop()
{
    imu_test.rotate();
    imu_test.read_gyroscope();
    altimeter.read_altitude();
    data.curtime((float)millis());
    data.readGPS();
    data.encodeAndAdd();
//    Serial.println(data.curtime());
    Serial.println(data.accelx());
}
