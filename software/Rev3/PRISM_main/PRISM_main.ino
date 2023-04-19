#include "data_class.h"
#include "imu_class.h"
#include "altimeter_class.h"
#include "kalman_class.h"
#include "led_class.h"
#include <Arduino.h>


// pinmodes
#define KEYSWITCH A12
#define MAIN 24
#define DROG 25


byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data

short int phase = 1;
float minimumAltitude;
float minimumDrogAltitude;
float minimumMainAltitude; // set on launch day


// setup sensors
Led statusLed(34);
Data data(&statusLed);
Imu imu_test(&data);
Kalman kalman_filter(&data);
Altimeter altimeter(&data);


void setup()
{
  pinMode(DROG,OUTPUT);
  pinMode(MAIN,OUTPUT);
  digitalWrite(DROG, LOW);  //very important!!
  digitalWrite(DROG, LOW);  //very important!!
  
  Serial.begin(115200);

  // begin sensors
  data.SDbegin();
  imu_test.begin_imu();
  altimeter.begin_altimeter();

  kalman_filter.begin();
    
}

// void loop()
// {
//     imu_test.rotate();
//     imu_test.read_gyroscope();
//     altimeter.read_altitude();
//     altimeter.read_temperature();
//     data.curtime((float)millis());
//     data.readGPS();
//     data.analogTelem();
//
////     Serial.print(data.temp());
//
//      
//     statusLed.RGB(0, 100, 0, 0);
//      statusLed.RGB(1, 100, 0, 0);
//
//
//
//     kalman_filter.update();
//
//     Serial.println(data.kfvz());
//     
//     data.encodeAndAdd();
//
//
//     delay(10);
// }

void loop()
{
  // Chooses loop to run through depending on what the phase is set to
  switch (phase)
  {

  // Prelaunch phase 1: Before keyswitch
  case 1:
    Serial.println("Phase 1:");
    while (phase == 1)
    {
      PreARM();
    }
    break;
 
  // Prelaunch phase 2: After keyswitch
  case 2:
    Serial.println("Phase 2:");
//    newcalltime = millis();
    while (phase == 2)
    {
      PostARM();
    }
    break;

  // Lauched but before apogee
  case 3:
    Serial.println("Phase 3:");
    while (phase == 3)
    {
      BeforeApogee();
    }
    break;

  // After apogee before main deploy
  case 4:
    Serial.println("Phase 4:");
    while (phase == 4)
    {
      BeforeMain();
    }
    break;

    // After main deploy
  case 5:
    Serial.println("Phase 5:");
    while (phase == 5)
    {
      AfterMain();
    }
  }
}


// launch phase functions

// phase 1
void PreARM()
{
  collect_data();

  // when keyswitch is turned, enter PostARM phase
  if (analogRead(KEYSWITCH) >= 1000)
  {
    int average_altitude = 0;
    int number_readings = 5;
    for (int i = 0; i < number_readings; i++){
      average_altitude += data.baralt();
    }

    minimumAltitude = (average_altitude / number_readings) + 2;// m //CHANGE BEFORE LUANCH
    phase = 2;
  }
  delay(100);
}

// phase 2
void PostARM()
{
  collect_data();
  status_lights();
  
  if (data.baralt() > minimumAltitude){
    // launch
    phase = 3;
  }
  if (analogRead(KEYSWITCH) <= 20){
    // turned off
    phase = 1;
  }
  delay(10);
}

// phase 3
void BeforeApogee()
{ 
  collect_data();
  status_lights();
  kalman_filter.update();
  if (data.kfvz() < -0.3) {
    // deploy drog
    digitalWrite(DROG, HIGH);
    delay(1000);
    digitalWrite(DROG, LOW);
    phase = 4;  
  }
  delay(10);
}

// phase 4
void BeforeMain()
{ 
  collect_data();
  status_lights();
  kalman_filter.update();
  if (data.baralt() < minimumMainAltitude){
    // deploy main parachute
    digitalWrite(MAIN, HIGH);
    delay(1000);
    digitalWrite(MAIN, LOW);
    phase = 5;
  }
  delay(10);
}

// phase 5
void AfterMain(){
  collect_data();
  status_lights();    
  kalman_filter.update();
  delay(10);    
}


// helper functions

// collect data function
void collect_data (void){
  // contains all sensor callings
  imu_test.rotate();
  imu_test.read_gyroscope();
  altimeter.read_altitude();
  altimeter.read_temperature();
  data.curtime((float)millis());
  data.readGPS();
  data.analogTelem();
  data.encodeAndAdd();

}

// neopixels
void status_lights (void) {
  statusLed.RGB(0, 100, 0, 0);
  statusLed.RGB(1, 100, 0, 0);
}
