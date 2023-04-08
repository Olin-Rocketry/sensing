#include "gps_class.h"

Gps::Gps(Data *data)
{
    this->data = data;
    init();
}

void Gps::init()
{
    //  Serial.begin(9600);
    test_connection();
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

void Gps::read_data()
{
    // display time

    // display location
    if (gps.location.isValid())
    {

        data->lng(gps.location.lng());
        data->lat(gps.location.lat());
    }
    if (gps.altitude.isValid())
    {
        data->gpsalt(gps.altitude.meters());
    }
}
