#include <Wire.h>
#include <SD.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "SparkFunMPL3115A2.h"
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
MPL3115A2 baro;

File myFile;

bool launch = false;
int led = 13;
unsigned long launchTime, startTime, openTime, closeTime;
const int chipSelect = BUILTIN_SDCARD;
const int batchSize = 3000;//2200;

const int smoothingFactor = 2;
const int numValues = 20;
float prevEMA = -100;
const int emaWeight = smoothingFactor / (1 + numValues);

int accelTriggerThresh = 50; //Threshold acceleration to sense launch (m/s^2)

#include "dataPoint.h"
//typedef struct dataPoint {
//  unsigned long timeSinceLaunch;
//  float acceleration[3];
//  float altitude, pressure, temp, filteredAltitude;
//} dataPoint;

dataPoint dataPoints[batchSize];
int currDataPoint = 0;

float filtered_altitude = 0;

void setup(void)
{
  Serial.begin(9600);
  Wire.begin();
  //Serial.println("Accelerometer Test"); Serial.println("");
  baro.begin(); // Get sensor online

  //Configure the sensor
  baro.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  baro.setOversampleRate(3); // Set Oversample to the recommended 128
  baro.enableEventFlags(); // Enable all three pressure and temp event flags

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    //Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(!accel.begin()){
              digitalWrite(led, LOW);
              delay(500);
              digitalWrite(led, HIGH);
    }
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);

}

void loop(void)
{

 while(!launch){
  checkLaunch();
  //Serial.println("Checking");
 }
 if (currDataPoint >= batchSize - 1){
  writeSensorData();
  currDataPoint = 0;
 }
 getIMU();
 getBaro();
 getFilteredAltitude(); //Must be called after getBaro();
 dataPoints[currDataPoint].timeSinceLaunch = millis()-launchTime;
 // Time Since Launch (ms), X Accel (m/s^2), Y Accel (m/s^2), Z Accel (m/s^2), Pressure (Pascals), Altitude (m), Temp (*F)
 currDataPoint++;

//  if(currDataPoint > 0) {
//    Serial.print("time difference: ");
//    Serial.println(dataPoints[currDataPoint - 1].timeSinceLaunch - dataPoints[currDataPoint - 2].timeSinceLaunch);
//  }
//   Serial.print("count = ");
//  Serial.println(currDataPoint);
}


void checkLaunch(void)
{
  //Serial.println("Checking launch");
  sensors_event_t event;
  accel.getEvent(&event);


  if (max(max(abs(event.acceleration.x),abs(event.acceleration.y)),abs(event.acceleration.z))>accelTriggerThresh)
  {
    launchTime = millis();
    //Serial.println("Takeoff!");
    digitalWrite(led, LOW);
    launch = true;
    delay(50);
  }
}

void getIMU(void)
{
  sensors_event_t event;
  accel.getEvent(&event);
  dataPoints[currDataPoint].acceleration[0] = event.acceleration.x;
  dataPoints[currDataPoint].acceleration[1] = event.acceleration.y;
  dataPoints[currDataPoint].acceleration[2] = event.acceleration.z;
}

void getBaro(void)
{
  dataPoints[currDataPoint].altitude = baro.readAltitudeFt();

  dataPoints[currDataPoint].pressure = baro.readPressure();

  dataPoints[currDataPoint].temp = baro.readTempF();
}


void writeSensorData(void)
{
  Serial.println("Writing to SD card");
  startTime = millis();
  // open the file.
  digitalWrite(led, HIGH);
  if (!SD.begin(chipSelect)) {
    //Serial.println("initialization failed!");
    return;
  }

  myFile = SD.open("flight1.txt", FILE_WRITE);//"Nov21ArcasH130WFlight1.txt", FILE_WRITE);
  openTime = millis();
  Serial.print("time to open SD card: ");
  Serial.println(openTime - startTime);


  for (int h = 0; h < batchSize; h++)
  {
    myFile.write((const uint8_t *)&dataPoints[h], sizeof(dataPoint));
  }

  closeTime = millis();
  Serial.print("time to write to SD card: ");
  Serial.println(closeTime - openTime);

  myFile.close();
  digitalWrite(led, LOW);

  Serial.print("time to close SD card: ");
  Serial.println(millis() - closeTime);

  Serial.print("total time: ");
  Serial.println(millis() - startTime);
}

void getFilteredAltitude(void) 
{
  if (prevEMA == -100) {
    if (currDataPoint == numValues - 1) {
      for (int i = 0; i < numValues; i++)
      {
        prevEMA = prevEMA + dataPoints[i].altitude;
      }
      prevEMA = prevEMA / numValues;
      dataPoints[currDataPoint].filteredAltitude = prevEMA;
    }
    else {
      dataPoints[currDataPoint].filteredAltitude = dataPoints[currDataPoint].altitude; 
    }
  }
  else {
    prevEMA = dataPoints[currDataPoint].altitude * emaWeight + prevEMA * (1 - emaWeight);
    dataPoints[currDataPoint].filteredAltitude = prevEMA;
  }
}

bool isApogee(void)
{
 if (dataPoints[currDataPoint].timeSinceLaunch < 5000 || currDataPoint <= 20) {
  return false;
 } else {
  return dataPoints[currDataPoint].filteredAltitude < dataPoints[currDataPoint - 20].filteredAltitude;
 }
}
