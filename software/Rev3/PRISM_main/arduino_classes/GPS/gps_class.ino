#include "gps_class.h"

Gps::gps() {
  init();
}

void Imu::init() {
  Serial.begin(9600);
  test_connection();
}

void Gps::test_connection() {
  // throw error if no connection
  if (!i2c_gps.begin()) {
    Serial.print("Ooops, no gps detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
}


void Gps::read_position() {
   while (i2c_gps.available()){
    gps.encode(i2c_gps.read());
  }

  if (gps.time.isUpdated()){
    print_data();
  }
}


void Gps::print_data() {
  // display time
  if (gps.time.isValid())
  {
    Serial.print((" Time: "));
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());

    Serial.println(); //Done printing time
  }
  else
  {
    Serial.println(F("Time not yet valid"));
  }

  // display location
  if (gps.location.isValid())
  {
    Serial.print("Location: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(", "));
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }
  else
  {
    Serial.println(F("Location not yet valid"));
  }


}
