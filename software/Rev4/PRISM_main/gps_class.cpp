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
    statusLed->RGB(1, 255, 0, 0);


    
    test_connection();
}

void Gps::test_connection()
{
    // throw error if no connection
    if (!i2c_gps.begin(Wire, 400000))
    {
        statusLed->RGB(1, 255, 0, 0);
        Serial.print("Ooops, no gps detected ... Check your wiring or I2C ADDR!");
        gpsStruct.diagmsg = pow(2, 2);
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
    
    if (gps.location.age() > 100) {
        gpsStruct.lng = 3.14;
        gpsStruct.lat = 3.14;
        gpsStruct.diagmsg = pow(2,2);
        statusLed->RGB(1, 255, 0, 255);
    }
    //radio->EAST_serial.sendDatum(gpsStruct);

    Serial8.flush();
}

void Gps::read_data()
{
    // display location
//    unsigned long t_1 = micros();

    if (gps.location.isValid())
    {
        statusLed->RGB(1, 0, 255, 0);
        gpsStruct.lng = gps.location.lng();
        gpsStruct.lat = gps.location.lat();
        gpsStruct.diagmsg = 0;
        

//        
//        Serial.print(gps.location.lng());
//        Serial.print(",");
//        Serial.print(gps.location.lat());
        
    }
    else{
      statusLed->RGB(1, 255, 0, 255);
      gpsStruct.lng = 3.14;
      gpsStruct.lat = 3.14;
      gpsStruct.diagmsg = pow(2, 2);
    }
    
    if (gps.altitude.isValid())
    {
        gpsStruct.gpsalt = gps.altitude.meters();
    }
    else{
        gpsStruct.diagmsg = pow(2, 2);
    }
//    radio->EAST_serial.sendDatum(gpsStruct);
//
//    Serial8.flush();

//    Serial.println(micros() - t_1);

    
}
