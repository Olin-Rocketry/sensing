#include "gps_class.h"

Gps::Gps(Radio *radio){
  this->radio = radio;
    init();
}

void Gps::init()
{
}

void Gps::begin_gps(Led *statusLed)
{
    Wire.begin();
    Serial.begin(9600);
    this->statusLed = statusLed;
    statusLed->RGB(0, 100, 0, 0);


    
    test_connection();
}

void Gps::test_connection()
{
    // throw error if no connection
    if (!i2c_gps.begin(Wire, 400000))
    {
        Serial.print("Ooops, no gps detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    delay(10);
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
    // display location
    if (gps.location.isValid())
    {
        statusLed->RGB(0, 0, 100, 0);
        gpsStruct.lng = gps.location.lng();
        gpsStruct.lat = gps.location.lat();
        Serial.print(gps.location.lng());
        Serial.print(",");
        Serial.println(gps.location.lat());
    }
    if (gps.altitude.isValid())
    {
        gpsStruct.gpsalt = gps.altitude.meters();
    }

//    radio->EAST_serial.sendDatum(gpsStruct);
}
