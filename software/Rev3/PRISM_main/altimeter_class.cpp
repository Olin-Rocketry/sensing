#include "altimeter_class.h"

Altimeter::Altimeter(Data *data)
{
    this->data = data;
    init();
}

void Altimeter::begin_altimeter()
{
    Wire.begin();
//    Serial.begin(9600);
}

void Altimeter::init()
{
    Serial.println("Adafruit BMP388 / BMP390 test");

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
//    if (!bmp.performReading())
//    {
//        Serial.println("Failed to perform reading :(");
//        return;
//    }
//    bmp.performReading();
//
//    /*Serial.print("Pressure = ");
//    Serial.print(bmp.pressure / 100.0);
//    Serial.println(" hPa");
//
//    Serial.println();
//    delay(1000);*/
}

void Altimeter::perform_reading()
{
  if (!bmp.performReading())
  {
      Serial.println("Failed to perform reading :(");
      return;
  }
  read_altitude();
  read_temperature();
}

void Altimeter::read_altitude()
{
//    if (!bmp.performReading())
//    {
//        Serial.println("Failed to perform reading :(");
//        return;
//    }
//    bmp.performReading();
//
//    /*Serial.print("Approx. Altitude = ");
//    Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
//    Serial.println(" m");*/

//    data->baralt((float)bmp.readAltitude(SEALEVELPRESSURE_HPA));
    data->baralt(44330.0 * (1.0 - pow((bmp.pressure / 100.0F) / SEALEVELPRESSURE_HPA, 0.1903)));
}

void Altimeter::read_temperature()
{
//    if (!bmp.performReading())
//    {
//        Serial.println("Failed to perform reading :(");
//        return;
//    }
//    bmp.performReading();
//
//    /*Serial.print("Temperature = ");
//    Serial.print(bmp.temperature);
//    Serial.println(" *C");*/

    data->temp((float)bmp.temperature);
}
void Altimeter::EMA(){
  
if (EMA_prev==-1){
  EMA_prev=data->baralt();
}
else{
  EMA_value=data->baralt()*(Smoothing/(1+Sample))+EMA_prev*(1-Smoothing/(1+Sample)); //Exponential moving Average
}

}
