#include "data_class.h"
#include "imu_class.h"
#include "altimeter_class.h"
#include "kalman_class.h"
#include "led_class.h"
#include <Arduino.h>


// pinmodes
#define KEYSWITCH A12
// PYRO 1 (top) = PIN 24
// PYRO 2 (bottom) = PIN 25
#define MAIN 24
#define DROGUE 25


byte IICdata[5] = {0, 0, 0, 0, 0}; // buffer for sensor data
bool debugEnable=false; //Enable debug printing
bool noSD = false;  // Enable debug without sd
short int phase = 1;
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
Led statusLed(34);
Data data(&statusLed);
Imu imu_test(&data);
Kalman kalman_filter(&data);
Altimeter altimeter(&data);


void setup()
{
  
  pinMode(DROGUE,OUTPUT);
  pinMode(MAIN,OUTPUT);
  digitalWrite(DROGUE, LOW);  //very important!!
  digitalWrite(MAIN, LOW);  //very important!!
  while(analogRead(KEYSWITCH) >= 100){
    tone(33, 700, 100);
    delay(200);
  };
  
  Serial.begin(115200);
  Serial.println("To enter debug mode, send any character over Serial; proceeding without debug mode in:");
  delay(5000);
  int countdown=25;
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
    debugPhase();
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
    tone(33, 400, 1000);
    kalman_filter.begin();
    float average_altitude = 0;
    int number_readings = 5;

    //take 5 reading and compute the averadge, then use this to set the launch detection altitude
    for (int i = 0; i < number_readings; i++){
      average_altitude += data.baralt();
      altimeter.perform_reading();
      
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

  //if below apogee velocity threshold, power the drogue pyro
  if (data.kfvz() < DROGUE_velocity_trigger) {
    // deploy drogue
    pyro_t_start = data.curtime();
    digitalWrite(DROGUE, HIGH);
//    tone(33, 1000);
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
    phase = 5;
    debugPhase();
  }
}

// phase 5
void AfterMain(){
  collect_data();
  status_lights();    
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
  data.diagmsg_reset(); // clear out previous diagnostic messages
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
