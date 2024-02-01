#include "gps_class.h"

Gps::Gps()
{
    init();
}

void Gps::init()
{
    //  Serial.begin(9600);
    test_connection();
    Serial2.begin(115200);
    mySend.begin(Serial2);
}

void Gps::begin_gps()
{
    Wire.begin();
    Serial.begin(9600);
}

void Gps::test_connection()
{
    // throw error if no connection
    if (!i2c_gps.begin(Wire, 400000))
    {
        Serial.print("Ooops, no gps detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    delay(1000);
}

void Gps::read_position()
{
    while (i2c_gps.available())
    {
        gps.encode(i2c_gps.read());
    }

    if (gps.time.isUpdated())
    {
        read_data();
    }
}

struct __attribute__((packed)) STRUCT {
  float lat;
  float lng;
  float gpsalt;
} gpsStruct;

void Gps::read_data()
{
    // display location
    if (gps.location.isValid())
    {
        gpsStruct.lng=gps.location.lng();
        gpsStruct.lat=gps.location.lat();
//        Serial.print(gps.location.lng());
//        Serial.print(",");
//        Serial.println(gps.location.lat());
    }
    if (gps.altitude.isValid())
    {
        gpsStruct.gpsalt=gps.altitude.meters();
    }
    mySend.sendDatum(gpsStruct);
}
