#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_MPL3115A2.h>
#include <utility/imumaths.h>

// Define sensors
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_MPL3115A2 mpl;

// Puts the rocket in the calibration phase (phase 1)
// There are 5 phases: Calibration, Pre-Launch, Launch, Detection of Apogee, Detection of 1,000 feet on descent
int phase = 1;

// Configure timers for aquiring sensor data
unsigned long lastCallTime = millis();

// Phase 1 Constants
int calibrationPhaseInterval = 100; // milliseconds

// Phase 2 Constants
int preLaunchPhaseInterval = 30; // milliseconds

  // Minimum acceleration and altitude required to start launch phase
  int minimumAcceleration = 10; // m/s/s
  int minimumAltitude = 100; // m

void setup(void)
{  

  Serial.begin(115200);
  
  // Check if sensors are detected
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1);
  }
  if (!mpl.begin()) {
    Serial.println("NO MPL3115A2 detected");
    while(1);
  }
  
  // Configure LED pins
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  // Configure Sensors
  mpl.setSeaPressure(1013.26);
  
}

void loop() {

  // Chooses loop to run through depending on what the phase is set to
  switch (phase) {

    // Calibration phase
    case 1:
      Serial.println("Phase 1:");
      while (phase == 1) {
        
        if (millis() - lastCallTime > calibrationPhaseInterval) {
          calibrationPhase();
          lastCallTime = millis();
        }
        
      }

    // Pre-Launch phase
    case 2:
      Serial.println("Phase 2:");
      while (phase == 2) {
        if (millis() - lastCallTime > preLaunchPhaseInterval) {
          preLaunchPhase();
          lastCallTime = millis();
        }
      }

    // Launch phase
    case 3:
      Serial.println("Phase 3:");
      while (phase == 3) {
        
      }
      
  }

}

// First Phase
void calibrationPhase(void)
{

  // Get Calibration values for system, gyroscope, accelerometer, and magnetometer
  // Calibration values go from 0 to 3, 3 meaning fully calibrated
  // LED lights will activate when gyroscope, accelerometer, and magnetometer are fully calibrated (3 lights)
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  // Displays calibration values
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);

  // LEDs will activate when sensors are calibrated
  if (gyro == 3) {
    digitalWrite(6, HIGH);
  }

  if (accel == 3) {
    digitalWrite(7, HIGH);
  }

  if (mag == 3) {
    digitalWrite(8, HIGH);
  }

  // Switches to pre-launch phase when all sensors are calibrated
  if (system == 3 && gyro == 3 && accel == 3 && mag ==3) {
    phase = 2;
  }
  
}

float fetchAccelerometerData() {

  // Fetches linear acceleration data (ignores gravity)
  imu::Vector<3> gyroscopeData = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> accelerometerData = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  // Calculates the true acceleration in the upward direction
  float xAccel = accelerometerData.x()*cos((90 + gyroscopeData.x())*(M_PI/180));
  float trueZAccelerationRelX = -xAccel*cos((90 + gyroscopeData.y())*(M_PI/180));
  float yAccel = accelerometerData.y()*sin((90 - gyroscopeData.x())*(M_PI/180));
  float trueZAccelerationRelY = yAccel*cos((90 - gyroscopeData.z())*(M_PI/180));
  float zAccel = accelerometerData.z()*sin((90 - gyroscopeData.y())*(M_PI/180));
  float trueZAccelerationRelZ = -zAccel*sin((90 - gyroscopeData.z())*(M_PI/180));

  float totalZAcceleration = trueZAccelerationRelX + trueZAccelerationRelY + trueZAccelerationRelZ;
  Serial.print("True Z Acceleration: ");
  Serial.print(totalZAcceleration);
  Serial.println(" m/s/s");

  return(totalZAcceleration);
  
}

float fetchAltimeterData() {

  // Fetches altitude data
  float altitude = mpl.getAltitude(); // m
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  return(altitude);
  
}

// Second Phase
void preLaunchPhase() {
  
  float acceleration = fetchAccelerometerData();
  float altitude = fetchAltimeterData();

  if (acceleration > minimumAcceleration && altitude > minimumAltitude) {
    phase = 3;
  }
  
}

// TODO: Implement Kalman Filter with fetched values
void kalmanFilter() {
  
}