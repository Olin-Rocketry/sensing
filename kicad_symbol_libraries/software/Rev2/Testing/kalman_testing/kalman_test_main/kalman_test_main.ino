//General
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>//General sensor library
#include <Adafruit_BNO055.h>//IMU
#include <Adafruit_MPL3115A2.h>//Altitude
#include <utility/imumaths.h>//Math
#include "phoenix_IV_functions.h"
#include "dataPoint.h"//Datapoint
#include <Adafruit_GPS.h> //GPS
#include <RH_RF95.h>//Radio
#define RFM95_CS 10//Radio
#define RFM95_RST 2//Radio
#define RFM95_INT 3//Radio
#define RF95_FREQ 915.0//Radio Change to 434.0 or other frequency, must match RX's freq!
#define GPSSerial Serial1//GPS
#define GPSECHO false//GPS
#define buzzer 4//buzzer to teensy pin 4
//Create Instances of sensors
RH_RF95 rf95(RFM95_CS, RFM95_INT);//Radio Singleton instance of the radio driver
Adafruit_GPS GPS(&GPSSerial);//GPS Connect to the GPS on the hardware port
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);//IMU
Adafruit_MPL3115A2 mpl;//Altimeter

//Creates list datapoint objects for flight during phase 3
const int batchSize = 500; 
dataPoint dataPoints[batchSize];
int currentDataPoint = 0;
//  char buffer1[70];
//Global Variables
File myFile; //SD
float IMU[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float altitude = 0.0;
float GPSArray[2] = {0.0, 0.0};

const float Pi = 3.14159;
// Puts the rocket in the calibration phase (phase 1)
// There are 5 phases: Calibration, Pre-Launch, Launch, Detection of Apogee, Detection of 1,000 feet on descent
short int phase = 0;
// Configure timers for aquiring sensor data
unsigned int long lastCallTime = millis();
// Phase 1 Constants
unsigned int calibrationPhaseInterval = 100; // milliseconds
// Phase 2 Constants
unsigned int preLaunchPhaseInterval = 30; // milliseconds
// Phase 3 Constants
unsigned int beforeApogeePhaseInterval = 30; // milliseconds
// Phase 4 Constants
unsigned int afterApogeePhaseInterval = 30; // milliseconds
// Phase 5 Constants
unsigned int afterMainDeploymentPhaseInterval = 30; // milliseconds
// Minimum acceleration and altitude required to start launch phase
float minimumAltitude;
float minimumDrogAltitude ;
float minimumMainAltitude;

double state[3];//{altitude, velocity, acceleration}, set to initial altitude, 0, initial global z accel on first run 

//TODO: if change this value, also change Q in kalman_update
double p_cov[3][3] = {{3, 0, 0}, {0, 2, 0}, {0, 0, 1}};
bool firstKalman = true;

void setup()
{  
  Serial.begin(9800);
  Serial.println("working");
  setupSensors();
}

bool gatherData = false;

void loop() {
    
  // Chooses loop to run through depending on what the phase is set to
  switch (phase) {

    // Calibration phase
    case 0:
      Serial.println("Phase 0:");
      
      
      while (phase == 0) {
        
        if (millis() - lastCallTime > calibrationPhaseInterval) {
          
          calibrationPhase();
          lastCallTime = millis();
        }
        
      }

    // Pre-Launch phase
    case 1:
      Serial.println("Phase 1:");
      while (phase == 1) {
        if (millis() - lastCallTime > preLaunchPhaseInterval) {

                fetchAltimeterData();
                
                minimumAltitude = altitude +0.1; // m //CHANGE BEFORE LUANCH
                minimumMainAltitude =  altitude+1; // m //CHANGE BEFORE LUANCH
              //Buzzer Pin 4
              //tone(buzzer, 1000); // Send 1KHz sound signal...
              GPSArray[0] = 1.0;

              phase = 2;

     }
    }
    // After Keyswitch before luanch
    case 2:
      Serial.println("Phase 2:");
      while (phase == 2) {
        if (millis() - lastCallTime > preLaunchPhaseInterval) {
          fetchAltimeterData();
          Serial.print(altitude);
          Serial.print("\t");
          Serial.println(minimumAltitude);
            if (altitude > minimumAltitude) {
                phase = 3;
                //noTone(buzzer); 
              }
        }
      }
    // Lauched but before apogee
    case 3:
      Serial.println("Phase 3:");
     
      while (phase == 3) {
        //if (millis() - lastCallTime > beforeApogeePhaseInterval) {
        fetchSensorData();
          addDataPoint();
          tone(buzzer,500);
   
          lastCallTime = millis();
          //if the Kalman esitmated velocity goes negative, trigger apogee procedures
          if (state[1] < 0){
            //trigger the droge e-match fet
            digitalWrite(A19, HIGH);
            delay(1000);
            digitalWrite(A19, LOW);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            digitalWrite(8, HIGH);
            noTone(buzzer);
            lastCallTime = millis();
            phase = 4;
            SD_write();
            //tone(buzzer, 1000);
          }
        
      }
      
    // After apogee before main deploy
    case 4:
      Serial.println("Phase 4:");
      currentDataPoint = 0;
      while (phase == 4) {
        if(currentDataPoint > 50){
          SD_write();
          digitalWrite(7, LOW);

       
        }
        if (millis() - lastCallTime > afterApogeePhaseInterval) {
          fetchSensorData();
          addDataPoint();
          lastCallTime = millis();
        }
      }
      
      }
}
