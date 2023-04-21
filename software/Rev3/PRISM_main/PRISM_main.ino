#include "data_class.h"
#include "imu_class.h"
#include "altimeter_class.h"
#include "kalman_class.h"
#include "led_class.h"
#include <Arduino.h>


// pinmodes
#define KEYSWITCH A12
#define MAIN 24  //PYRO 1 (top)
#define DROG 25  //PYRO 2 (bottom)


byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data
bool debugEnable=false; //Enable debug printing
short int phase = 1;
unsigned long loop_t_start = 0;
unsigned long loop_t_end = 0;
float minimumAltitude;
float minimumDrogAltitude;
float minimumMainAltitude = 5; // set on launch day
float trigger_offset = 5; // m set on launch day
float drog_velocity_trigger = -1; //m/s set on launch day


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
  while(analogRead(KEYSWITCH) >= 100){
    tone(33, 800, 100);
    delay(200);
  };
  
  Serial.begin(115200);
  Serial.println("To enter debug mode, send any character over Serial; proceeding without debug mode in:");
  int countdown=5;
  while(Serial.available()==0)
  {
    Serial.println(countdown--);
    delay(750);
    if(millis()>5000)
    {
      break;
    }
  }
  if(Serial.available()!=0)
  {
    debugEnable=true;
    Serial.println("Entered debug mode");
    debugPhase();
  }
  else
  {
    Serial.println("Debug mode not enabled; proceeding normally");
  }
  // begin sensors
  data.SDbegin(debugEnable);
  imu_test.begin_imu(debugEnable);
  altimeter.begin_altimeter(debugEnable);


  data.kfx(drog_velocity_trigger);
}

void loop()
{
  loop_t_start = micros();
  
  // Chooses loop to run through depending on what the phase is set to
  switch (phase)
  {

    // Prelaunch phase 1: Before keyswitch
    case 1:
      PreARM();
      break;
   
    // Prelaunch phase 2: After keyswitch
    case 2:
      PostARM();
      break;
  
    // Lauched but before apogee
    case 3:
      BeforeApogee();
      break;
  
    // After apogee before main deploy
    case 4:
      BeforeMain();
      break;
  
      // After main deploy
    case 5:
      AfterMain();
      break;
      
  }
  
  loop_t_end = micros();
  // adjust delay based on length of loop
  if((loop_t_end - loop_t_start) < 10000){
    delayMicroseconds(10000 - (loop_t_end - loop_t_start));
  }  
}


// launch phase functions
// phase 1
void PreARM()
{
  collect_data();

  // when keyswitch is turned, enter PostARM phase
  if (analogRead(KEYSWITCH) >= 100)
  {
    tone(33, 600, 1000);
    kalman_filter.begin();
    int average_altitude = 0;
    int number_readings = 5;

    //take 5 reading and compute the averadge, then use this to set the launch detection altitude
    for (int i = 0; i < number_readings; i++){
      average_altitude += data.baralt();
    }
    minimumAltitude = (average_altitude / number_readings) + trigger_offset;
    data.kfy(minimumAltitude);
    phase = 2;
    debugPhase();
  }
}

// phase 2
void PostARM()
{
  collect_data();
  kalman_filter.update();


  
  status_lights();


  
  
  if (data.baralt() > minimumAltitude){
    // launch

    
    phase = 3;
    debugPhase();
  }
  if (analogRead(KEYSWITCH) <= 100){
    // turned off
    phase = 1;
    debugPhase();
  }
}

// phase 3
void BeforeApogee()
{ 
  collect_data();
  status_lights();
  kalman_filter.update();
  if (data.kfvz() < drog_velocity_trigger) {
    // deploy drog
//    digitalWrite(DROG, HIGH);
//    delay(1000);
//    digitalWrite(DROG, LOW);
    phase = 4; 
    debugPhase(); 
  }
}

// phase 4
void BeforeMain()
{ 
  collect_data();
  status_lights();
  kalman_filter.update();
  if (data.baralt() < (minimumMainAltitude + minimumAltitude)){
    // deploy main parachute
//    digitalWrite(MAIN, HIGH);
//    delay(1000);
//    digitalWrite(MAIN, LOW);
    phase = 5;
    debugPhase();
  }
}

// phase 5
void AfterMain(){
  collect_data();
  status_lights();    
  kalman_filter.update();
}


// helper functions

// collect data function
void collect_data (void){
  // contains all sensor callings
  imu_test.perform_reading();
  altimeter.perform_reading();
  data.curtime((float)millis());
  data.readGPS();
  data.phs(phase);
  data.analogTelem();
  data.encodeAndAdd();
}

// neopixels
void status_lights (void) {
  statusLed.RGB(0, 100, 0, 0);
  statusLed.RGB(1, 100, 0, 0);
}

void debugPhase(void)
{
  if(debugEnable==true)
  {
    Serial.print("Phase ");
    Serial.println(phase);
  }
}
