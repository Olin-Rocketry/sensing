#include "data_class.h"
#include "imu_class.h"
#include "altimeter_class.h"
#include "kalman_class.h"
#include "led_class.h"
#include <Arduino.h>


//pinmodes
#define Main 24
#define Drog 25

  



// setup for testing
byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data

short int phase = 1;
float minimumAltitude;
float minimumDrogAltitude;
float minimumMainAltitude;




Led statusLed(34);
Data data(&statusLed);
Imu imu_test(&data);
Kalman kalman_filter(&data);
// Altimeter altimeter(&data);

void setup()
{
  pinMode(Drog,OUTPUT);
  pinMode(Main,OUTPUT);
  digitalWrite(Drog, LOW);  //very important!!
  digitalWrite(Main, LOW);  //very important!!

  Serial.begin(115200);
  
  data.SDbegin();
  imu_test.begin_imu();
  
//  altimeter.begin_altimeter();
  //    test.test_connection();

  //    led.RGB(0, 0, 0, 100);
  //    led.RGB(1, 0, 0, 100);

//  kalman_filter.begin();
}

 void loop()
 {
     imu_test.rotate();
     imu_test.read_gyroscope();
 //    altimeter.read_altitude();
     data.curtime((float)millis());
     data.readGPS();
     data.analogTelem();



//     kalman_filter.update();
     
     data.encodeAndAdd();
 //    Serial.println(data.curtime());
 //    Serial.print("Accel x: ");
 //    Serial.println(data.accelx());
 //    Serial.print("Bar alt: ");
 //    Serial.println(data.baralt());

     delay(90);
 }

//void loop()
//{
//  // Chooses loop to run through depending on what the phase is set to
//  switch (phase)
//  {
//
//  // Prelaunch phase 1: Before keyswitch
//  case 1:
//    Serial.println("Phase 1:");
//    while (phase == 1)
//    {
//      PreARM();
//    }
//    break;
// 
//  // Prelaunch phase 2: After keyswitch
//  case 2:
//    Serial.println("Phase 2:");
//    newcalltime = millis();
//    while (phase == 2)
//    {
//      PostARM();
//    }
//    break;
//
//  // Lauched but before apogee
//  case 3:
//    Serial.println("Phase 3:");
//    while (phase == 3)
//    {
//      BeforeApogee();
//    }
//    break;
//
//  // After apogee before main deploy
//  case 4:
//    Serial.println("Phase 4:");
//    while (phase == 4)
//    {
//      BeforeMain();
//    }
//    break;
//
//    // After main deploy
//  case 5:
//    Serial.println("Phase 5:");
//    while (phase == 5)
//    {
//      AfterMain();
//    }
//  }
//}
//
//// launch phase functions
//
//// phase 1
//void PreARM()
//{
//  data.readGPS();
//  altimeter.read_altitude();
//
//  // when keyswitch is turned, enter PostARM phase
//  if (analogRead(A12) >= 1000)
//  {
//    int averageAltitude=0;
//    int number_readings=5;
//    for (int i=0; i<number_readings; i++){
//      fetchAltimeterData();
//      sumAltitude=altitude+sumAltitude;
//      Serial.println(altitude);
//    }
//
//    minimumAltitude=sumAltitude/number_readings+2;// m //CHANGE BEFORE LUANCH
//    phase = 2;
//  }
//}
//
//// phase 2
//void PostARM()
//{
//  if (data.baroalt()> minimumAltitude){
//    // launch
//    phase = 3;
//  }
//  if (analogeRead(A12)<=20{
//    // turned off
//    phase = 1;
//  }
//}
//
//// phase 3
//void BeforeApogee()
//{ 
//   if (data.baroalt()> minimumDrogAltitude){
//    // after apogee
//    phase = 4;
//  }
//  
//}
//
//// phase 4
//void BeforeMain()
//{ 
//   if (data.baroalt() < minimumMainAltitude){
//    // deploy main parachute
//    // TODO: change A20 to actual pin number
//    digitalWrite(A20, HIGH);
//    delay(1000);
//    digitalWrite(A20, LOW);
//
//    phase = 5;
//    }
//    
//}
//
//// phase 5
// void AfterMain(){
//          
//}
//
//// collect data function
//void DataCollection{
//  // contains all sensor callings
//  altimeter.read_altitude();
//  
//}
