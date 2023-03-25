// test for reading imu data using oop
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class Imu_oop {
  // set up pins as private variables
  private:
    Adafruit_BNO055 bno;
    
  public:
    // create constructor in public portion
    // call init to initialize the pin modes
    Imu_oop() {
      this->bno = Adafruit_BNO055(55, 0x28);
      init();  
    }
    
    void init() {
      Wire.begin();
      Serial.begin(9600);
      test_connection();
    }

    void test_connection() {
      // throw error if no connection
      if (!bno.begin()) {
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
      }
      bno.setExtCrystalUse(true);
      delay(1000);
    }


    // data retrieval functions
    void read_quaternions() {
      imu::Quaternion quat = bno.getQuat();
      
      // Display the quat data
      Serial.print("qW: ");
      Serial.print(quat.w(), 4);
      Serial.print(" qX: ");
      Serial.print(quat.y(), 4);
      Serial.print(" qY: ");
      Serial.print(quat.x(), 4);
      Serial.print(" qZ: ");
      Serial.print(quat.z(), 4);
      Serial.println("");
    }

    void read_euler() {
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
      print_data(euler); 
    }

    void read_gravity() {
      imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
      print_data(gravity);
    }

    void read_gyroscope() {
      imu::Vector<3> gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
      print_data(gyroscope);
    }

    void read_accelerometer() {
      imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
      print_data(accel);
    }

    void read_linear_accel() {
      imu::Vector<3> linear_accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
      print_data(linear_accel);
    }

    void print_data(imu::Vector<3> data) {
      /* Display the floating point data */
      Serial.print("X: ");
      Serial.print(data.x());
      Serial.print(" Y: ");
      Serial.print(data.y());
      Serial.print(" Z: ");
      Serial.print(data.z());
      Serial.println("");  
    }

//    void read_data(String data_type) {
//      // data_type is a string in the form of 'DATA'
//      if (data_type == "QUATERNION") {
//        read_quaternions();
//      } else {
//        imu::Vector<3> data_values;
//
//        switch(data_type) {
//          "GYROSCOPE":
//            data_values = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
//            break;
//          "EULER":
//            data_values = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
//            break;
//          "ACCELERATION":
//            data_values = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
//            break;
//          "GRAVITY":
//            data_values = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
//            break;
//          "LINEAR_ACCELERATION":
//            data_values = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
//            break;
//          default:
//            Serial.println("Invalid data type; try again");
//        }
//        
//        print_data(data_values);
//      }
//    }
};



//setup for testing 
byte IICdata[5] = {0,0,0,0,0}; //buffer for sensor data

// declare instance of Imu as global for scope
Imu_oop test;


void setup() {
//  Wire.begin();
//  Serial.begin(9600);
}

void loop() {
//  test.test_connection();
  test.read_euler(); 
}
