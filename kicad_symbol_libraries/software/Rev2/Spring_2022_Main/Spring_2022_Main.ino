//General
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>//General sensor library
#include <Adafruit_BNO055.h>//IMU
//#include <Adafruit_MPL3115A2.h>//Altitude
#include <utility/imumaths.h>//MathM
#include "phoenix_IV_functions.h"
#include "dataPoint.h"//Datapoint
#include <RH_RF95.h>//Radio
#define RFM95_CS 10//Radio
#define RFM95_RST 2//Radio
#define RFM95_INT 3//Radio
#define RF95_FREQ 915.0//Radio Change to 434.0 or other frequency, must match RX's freq!
#define buzzer 4//buzzer to teensy pin 4
#define ALTMODE 1; //comment out for barometer mode; default is altitude mode
//start altitude to calculate mean sea level pressure in meters
#define ALTBASIS 32 //CHANGE BEFORE LUANCH
//this altitude must be known (or provided by GPS etc.)

const int SENSORADDRESS = 0x60; // address specific to the MPL3115A1, value found in datasheet
 
float altsmooth = 0; //for exponential smoothing
byte IICdata[5] = {0,0,0,0,0}; //buffer for sensor data
//Create Instances of sensors
RH_RF95 rf95(RFM95_CS, RFM95_INT);//Radio Singleton instance of the radio driver
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);//IMU
//Adafruit_MPL3115A2 mpl;//Altimeter

//Creates list datapoint objects for flight during phase 3
const int batchSize = 500; 
dataPoint dataPoints[batchSize];
int currentDataPoint = 0;

//Global Variables
File myFile; //SD
float IMU[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float altitude = 0.0;
float GPSArray[2] = {0.0, 0.0};

const float Pi = 3.14159;
// Puts the rocket in the calibration phase (phase 1)
// There are 6 phases: Calibration, pre keyswitch, Pre-Launch, Launch, Detection of Apogee, Detection of 1,000 feet on descent
short int phase = 0;
// Configure timers for aquiring sensor data
unsigned int long lastCallTime = millis();
unsigned int newcalltime;
// Minimum acceleration and altitude required to start launch phase
float minimumAltitude;
float minimumMainAltitude;

double state[3];//{altitude, velocity, acceleration}, set to initial altitude, 0, initial global z accel on first run 

//TODO: if change this value, also change Q in kalman_update
double p_cov[3][3] = {{3, 0, 0}, {0, 2, 0}, {0, 0, 1}};
bool firstKalman = true;
char buffer[50];
bool gatherData = false;

void setup(void)
{  
  Wire.begin();
  Serial.begin(9600);
  setupSensors();
}
void loop() {
    
  // Chooses loop to run through depending on what the phase is set to
  switch (phase) {

    // Calibration phase
    case 0:
      Serial.println("Phase 0:");
      while (phase == 0) {
          calibrationPhase();
          phase=1;
      }
      break;

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
