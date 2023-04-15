#include "data_class.h"
#include "imu_class.h"
#include "altimeter_class.h"
#include "led_class.h"
#include <Arduino.h>

// setup for testing
byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data

short int phase = 1;
float minimum_altitude;
float minimum_main_altitude;

Led statusLed(34);
Data data(&statusLed);
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

//void loop()
//{
//    imu_test.rotate();
//    imu_test.read_gyroscope();
//    altimeter.read_altitude();
//    data.curtime((float)millis());
//    data.readGPS();
//    data.encodeAndAdd();
////    Serial.println(data.curtime());
////    Serial.print("Accel x: ");
////    Serial.println(data.accelx());
////    Serial.print("Bar alt: ");
////    Serial.println(data.baralt());
//
//    
//    delay(100);
//}


void loop() {
// Chooses loop to run through depending on what the phase is set to
  switch (phase) {

    // Pre-Launch phase
    case 1:
      Serial.println("Phase 1:");
      while (phase == 1) {
          PreLaunch();
      }
      break;
    // After Keyswitch before luanch
    case 2:
      Serial.println("Phase 2:");
     newcalltime=millis();
      while (phase == 2) {
       AfterLaunch();
      }
      break;
      
    // Lauched but before apogee
    case 3:
      Serial.println("Phase 3:");
      while (phase == 3) {
          BeforeApogee();
      }
      break;
      
    // After apogee before main deploy
    case 4:
      Serial.println("Phase 4:");
      while (phase == 4) {
       BeforeMain();
      }
      break;
      
  //After main deploy
   case 5:
      Serial.println("Phase 5:");
        while (phase == 5) {
          AfterMain();
        }
      }  
}


// launch phase functions

void PreLaunch() {
//    data.readGPS();
//    if (data.gpsalt() >= 1000 {
//      int avg_altitude = 0;
//      int num_readings = 5;
//      for (int i=0; i<number_readings; i++){
//        avg_altitude = data.gpsalt() + avg_altitude;
//        Serial.println(data.gpsalt());
//      }
//      minimum_altitude = avg_altitude / num_readings + 2;
//      minimum_main_altitude = avg_altitude / num_readings + 5;
//      phase = 2;
//    }
  
  data.readGPS();
  altimeter.read_altitude();
  if (data.baralt() >= 1000) {
    phase = 2;  
  }
}

void AfterLaunch() {
  data.readGPS();  
}
