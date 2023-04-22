#include "altimeter_class.h"

Altimeter::Altimeter(Data *data)
{
    this->data = data;
    init();
}

void Altimeter::begin_altimeter(bool debugEnable)
{
    Wire.begin();
    this->debugEnable=debugEnable;
    if(debugEnable==true)
    {
      Serial.println("Altimeter started");
    }
}

void Altimeter::init()
{
    if (!bmp.begin_I2C())
    { // hardware I2C mode, can pass in address & alt Wire
        Serial.println("Could not find a valid BMP3 sensor, check wiring!");
        while (1)
            ;
    }

    bmp.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
    bmp.setPressureOversampling(BMP3_NO_OVERSAMPLING);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
    bmp.setOutputDataRate(BMP3_ODR_200_HZ);
}

void Altimeter::read_pressure()
{
  //This method has been deprecated.
}

void Altimeter::perform_reading()
{
  if (!bmp.performReading())
  {
      if(debugEnable==true)
      {
        Serial.println("Altimeter reading error");
      }
      //need to add some sort of fix to prevent faulty reading from stopping the kalman filter
//      data->baralt(data->baralt());
//      data->temp(data->temp());
      return;
  }
  read_altitude();
  read_temperature();
}

void Altimeter::read_altitude()
{
   data->baralt(44330.0 * (1.0 - pow((bmp.pressure / 100.0F) / SEALEVELPRESSURE_HPA, 0.1903)));

}

void Altimeter::read_temperature()
{
   data->temp((float)bmp.temperature);
}
void Altimeter::EMA(){ 
if (EMA_prev==-1){
  EMA_prev=data->baralt();
}
EMA_value=data->baralt()*(Smoothing/(1+Sample))+EMA_prev*(1-Smoothing/(1+Sample)); //Exponential moving Average
EMA_prev=EMA_value;
}
