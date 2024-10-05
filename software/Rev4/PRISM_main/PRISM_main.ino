#include "data_class.h"
#include "imu_class.h"
#include "altimeter_class.h"
#include "kalman_class.h"
#include "led_class.h"
#include "gps_class.h"
#include "radio_class.h"
#include "stepper_class.h"

#include <Arduino.h>


// pinmodes
#define KEYSWITCH A8
// PYRO 1 (top) = PIN 24
// PYRO 2 (bottom) = PIN 25
#define BUZZER 6
#define MAIN 24
#define DROGUE 25


byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data
bool debugEnable=true; //Enable debug printing
bool noSD = false;  // Enable debug without sd
short int flight_phase = 1;
unsigned long loop_t_start = 0;
unsigned long loop_t_end = 0;
float minimumAltitude;
float minimumDROGUEAltitude;
float minimumMainAltitude = 152; // 152 [meters] set on launch day (main deploy)
float trigger_offset = 30; // 30 [meters] set on launch day  (launch detect)
float DROGUE_velocity_trigger = -3; //-3[m/s] set on launch day (apogee detect)

float pyro_t_start = 0; //start time of pyro event
float beep_t_start = 0;
bool beep=false;


// setup sensors
Led statusLed1(0);
Data data(&statusLed1);
Imu imu_test(&data);
Kalman kalman_filter(&data);
Altimeter altimeter(&data);

//EAST
Radio radio(&data);
Gps gps(&radio);

StepperMotor steppermotor;

unsigned long cycle_time;
unsigned long old_cycle_time = 0;

void setup()
{
    //steppermotor.enable_stepper();
//  steppermotor.home_stepper();
  
  Serial.begin(115200);
  //tone(BUZZER,1500,1000);
  //radio.led_test(&statusLed1);
  delay(10);
  Serial.print("SD Card Present ");
  Serial.println(digitalRead(1));
  Serial.print(CrashReport);
  delay(1000);
  Serial.println("starting Radio");
  //radio.begin();
  Serial.println("Radio Started");
  gps.begin_gps(&statusLed1);
  
  pinMode(DROGUE,OUTPUT);
  pinMode(MAIN,OUTPUT);
  digitalWrite(DROGUE, LOW);  //very important!!
  digitalWrite(MAIN, LOW);  //very important!!
  while(analogRead(KEYSWITCH) >= 100){
    tone(BUZZER, 700, 100);
    delay(200);
  };
  
  Serial.println("To enter debug mode, send any character over Serial; proceeding without debug mode in:");
  delay(5000);
  int countdown=10;
  while(Serial.available()==0)
  {
    Serial.println(countdown--);
    delay(200);
    if(millis()>10000)
    {
      break;
    }
  }
  if(Serial.available()!=0)
  {
    debugEnable=true;
    Serial.println("Entered debug mode");
    noSD = true;
    Serial.println("Entered no SD mode");  
    debugflight_phase();
  }
  else
  {
    Serial.println("Debug mode not enabled; proceeding normally");
  }

  // begin sensors
  data.SDbegin(debugEnable, noSD);
  imu_test.begin_imu(debugEnable);
  altimeter.begin_altimeter(debugEnable);
  data.kfx(DROGUE_velocity_trigger);
}

void loop()
{
  loop_t_start = micros();
  //EAST
  cycle_time = millis() - old_cycle_time;
  
  if(cycle_time > 1000){
    
     gps.read_position();
     old_cycle_time = millis();
     
  }
 
  // radio

  radio.sendRadio();

  // stepper
  int t_now = millis();

  int pos = t_now/10000 * 360*20;

//  Serial.println(pos);
  
  steppermotor.move_stepper(pos);


  if (t_now > 1000*30){
    steppermotor.disable_stepper();
  }
  
  
  // Chooses loop to run through depending on what the flight_phase is set to
  switch (flight_phase)
  {

    // Prelaunch flight_phase 1: Before keyswitch
    case 1:
      PreARM();
      break;
   
    // Prelaunch flight_phase 2: After keyswitch
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


// launch flight_phase functions
// flight_phase 1
void PreARM()
{
  collect_data();

  // when keyswitch is turned, enter PostARM flight_phase
  if (analogRead(KEYSWITCH) >= 100)
  {
    tone(BUZZER, 400, 1000);
    kalman_filter.begin();
    float average_altitude = 0;
    int number_readings = 5;

    for (int freq = 400; freq <= 1000; freq += 100) {
        tone(BUZZER, freq, 100);
        delay(100);

    //take 5 reading and compute the averadge, then use this to set the launch detection altitude
    for (int i = 0; i < number_readings; i++){
      average_altitude += data.baralt();
      altimeter.perform_reading();
      
    }
    minimumAltitude = (average_altitude / number_readings) + trigger_offset;
    data.kfy(minimumAltitude);
    flight_phase = 2;
    debugflight_phase();
    }
    tone(BUZZER, 200, 3000);
  }
}

// flight_phase 2
void PostARM()
{
  collect_data();
  kalman_filter.update();


  
  statusLed1.RGB2(0, 255, 255, 0); // Yellow for LED 1
  //statusLed2.RGB2(1, 255, 255, 0); // Yellow for LED 2

  
  
  if (data.baralt() > minimumAltitude){
    // launch

    
    flight_phase = 3;
    debugflight_phase();
  }
  if (analogRead(KEYSWITCH) <= 100){
    // turned off
    flight_phase = 1;
    debugflight_phase();
  }
}

// flight_phase 3
void BeforeApogee()
{ 
  collect_data();
  status_lights();
  kalman_filter.update();

  //if below apogee velocity threshold, power the drogue pyro
  if (data.kfvz() < DROGUE_velocity_trigger) {
    // deploy drogue
    pyro_t_start = data.curtime();
    digitalWrite(DROGUE, HIGH);
//    tone(33, 1000);
    flight_phase = 4; 
    debugflight_phase(); 
  }
}

// flight_phase 4
void BeforeMain()
{ 
  collect_data();
  status_lights();
  kalman_filter.update();

  //if pyro has been on for 1s, turn it off
  if (data.curtime() > (pyro_t_start + 1000)){
    digitalWrite(DROGUE, LOW);
    noTone(33);
  }

  //if at main deploy, power the main pyro
  if ((data.curtime() > (pyro_t_start + 5000)) & (data.baralt() < (minimumMainAltitude + minimumAltitude))){
    // deploy main parachute
    pyro_t_start = data.curtime();
    digitalWrite(MAIN, HIGH);
//    tone(33, 500);
    flight_phase = 5;
    debugflight_phase();
  }
}

// flight_phase 5
void AfterMain(){
  collect_data();
  statusLed1.RGB2(0, 0, 255, 0); // Green for LED 1
  // statusLed2.RGB2(1, 0, 255, 0); // Green for LED 2

  kalman_filter.update();

  //if pyro has been on for 1s, turn it off
  if (!beep & (data.curtime() > (pyro_t_start + 1000))){
    digitalWrite(MAIN, LOW);
    noTone(33);
    beep=true;
  }
  if (beep & (data.curtime() > (beep_t_start + 4000)))
  {
    tone(33, 2000, 500);
    beep_t_start=data.curtime();
  }
  //loop?
  // First burst
//    tone(BUZZER, 1500, 1000); // 1.5 kHz tone for 1 second
//    delay(1000); // Wait for 1 second
//
//  // Second burst
//  tone(BUZZER, 1500, 1000); // 1.5 kHz tone for 1 second
//  delay(7000); // Wait for 7 seconds
}


// helper functions

// collect data function
void collect_data (void){
  // contains all sensor callings
  imu_test.perform_reading();
  altimeter.perform_reading();
  data.curtime((float)millis());
  data.readGPS();
  data.phs(flight_phase);
  data.analogTelem();
  data.encodeAndAdd();
  data.diagmsg_reset(); // clear out previous diagnostic messages
}

// neopixels
void status_lights (void) {
  statusLed1.RGB2(0, 100, 0, 0);
}

void debugflight_phase()
{
  if(debugEnable==true)
  {
    Serial.print("flight_phase ");
    Serial.println(flight_phase);
  }
}
