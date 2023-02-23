#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SparkFunMPL3115A2.h>

#define USE_LORA // 20% memory
#define USE_GPS // 26% memory
#define USE_LOGGING // 1% memory
#define USE_LOGGING_VERBOSE // TBD% memory

// pin definitions
#define RFM95_INT       2 // same as G0?
#define RFM95_RST       24
#define RFM95_CS        23
#define GPS_RX          30
#define GPS_TX          31

#ifdef USE_LORA
  #include <RH_RF95.h> // RadioHead library

  #define RF95_FREQ 915.0 // LORA frequency
  RH_RF95 rf95(RFM95_CS, RFM95_INT);   // radio object

  // packet timing
  long LAST_TELEMETRY = 0;
  int TELEMETRY_INTERVAL = 5000;
#endif

#ifdef USE_GPS
  #include <Adafruit_GPS.h>
  #include <SoftwareSerial.h>
  #define GPSSerial Serial5
//   SoftwareSerial softSerial(GPS_TX, GPS_RX);
  Adafruit_GPS GPS(&GPSSerial);
#endif

#include "dataPoint.h" // contains structures for data stored to SD and telemetry data
// Time Since Launch (ms), X Accel (m/s^2), Y Accel (m/s^2), Z Accel (m/s^2), Pressure (Pascals), Altitude (m), Temp (*F)

File myFile;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
MPL3115A2 baro;


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


dataPoint dataPoints[batchSize];
int currDataPoint = 0;

PACKET_UNION packet;

float filtered_altitude = 0;

void setup(void)
{
    Serial.begin(9600);
    Wire.begin();

    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);

    packet.data.timeSinceLaunch = 0;
    packet.data.drogueDeployed = 0;
    packet.data.mainDeployed = 0;

    setup_baro();
    setup_accel();
    setup_LORA();
    setup_GPS();
}

void loop(void)
{
    // still send telemetry while on the pad
    if(millis() - LAST_TELEMETRY > TELEMETRY_INTERVAL) {
        if (launch) {
            packet.data.timeSinceLaunch = dataPoints[currDataPoint].timeSinceLaunch;
        }
        #ifdef USE_GPS
            read_gps();
        #endif
        #ifdef USE_LORA
            send_LORA(); // send GPS and deployment data over radio
        #endif
    }

    if (!launch) {
        checkLaunch();
        //Serial.println("Checking");
        return;
    }
    if (currDataPoint >= batchSize - 1){
        writeSensorData(); // write the flight data stored in dataPoints
        currDataPoint = 0;
    }

    getAccel();
    getBaro();
    getFilteredAltitude(); //Must be called after getBaro();

    dataPoints[currDataPoint].timeSinceLaunch = millis()-launchTime;
    currDataPoint++;
}

void setup_baro () {
    baro.begin(); // Get sensor online

    //Configure the sensor
    baro.setModeAltimeter(); // Measure altitude above sea level in meters
    //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

    baro.setOversampleRate(3); // Set Oversample to the recommended 128
    baro.enableEventFlags(); // Enable all three pressure and temp event flags
}

void setup_accel() {
    if(!accel.begin()) {
        while(!accel.begin()) {
            digitalWrite(led, LOW);
            delay(500);
            digitalWrite(led, HIGH);
        }
    }
    /* Set the range to whatever is appropriate for your project */
    accel.setRange(ADXL345_RANGE_16_G);
}

void setup_GPS() {

  #ifdef USE_GPS
    // configure GPS serial
    GPS.begin(9600);

    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // set the update rate
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

    Serial.println(F("GPS initialized."));
 #endif
}

void setup_LORA() {

  #ifdef USE_LORA

    // set pins
    pinMode(RFM95_CS, OUTPUT); // set CS pins as outputs
    digitalWrite(RFM95_CS, HIGH); // set CS pins to disable

    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    while (!rf95.init()) {
      #ifdef USE_LOGGING
        Serial.println(F("LORA error."));
      #endif
    }

    while (!rf95.setFrequency(RF95_FREQ)) {
      #ifdef USE_LOGGING
        Serial.println(F("Frequency error."));
      #endif
    }
  #endif
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

void getAccel(void)
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
    if (dataPoints[currDataPoint].filteredAltitude < 600) {
      packet.data.mainDeployed = 1;
    }
  }
}

bool isApogee(void)
{
 if (dataPoints[currDataPoint].timeSinceLaunch < 5000 || currDataPoint <= 20) {
  return false;
 } else {
     for (int i = 1; i < 20; i++) {
         if (!dataPoints[currDataPoint].filteredAltitude < dataPoints[currDataPoint - i].filteredAltitude) {
             return false;
         }
     }
  packet.data.drogueDeployed = 1;
  return true;
 }
}

void read_gps() {
    char c = GPS.read();

    #ifdef USE_LOGGING_VERBOSE
        if (c) Serial.print(c);
    #endif

    //   if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived()) {
        #ifdef USE_LOGGING
        Serial.println(F("Got a GPS packet!"));
        #endif

        if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
        return;  // we can fail to parse a sentence in which case we should just wait for another
    }

    packet.data.gps_latitude = GPS.latitudeDegrees;
    packet.data.gps_longitude = GPS.longitudeDegrees;

    #ifdef USE_LOGGING
      Serial.println(F("Parsed a GPS packet!"));
      Serial.print(F("Location: "));
      Serial.print(packet.data.gps_latitude, 4);
      Serial.print(F(", "));
      Serial.println(packet.data.gps_longitude, 4);
    #endif
}

void send_LORA() {
    // send packet
    rf95.send(packet.raw, sizeof(packet.raw));
    // wait for packet to finish sending
    rf95.waitPacketSent();
}
