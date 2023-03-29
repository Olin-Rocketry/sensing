 #include "altimeter.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
 
Altimeter::Altimeter(void) {
  init();
}

void Altimeter::init() {
  Adafruit_BMP3XX bmp;
    Serial.println("Adafruit BMP388 / BMP390 test");

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void Altimeter::readPres(){
    if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  bmp.performReading();
  
  Serial.print("Pressure = ");
  Serial.print(bmp.pressure / 100.0);
  Serial.println(" hPa");

  
  Serial.println();
  delay(1000);
}

void Altimeter::readAlt(){
    if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  bmp.performReading();

  Serial.print("Approx. Altitude = ");
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  
  Serial.println();
  delay(1000);
  
}

void Altimeter::readTemp(){
    if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  bmp.performReading();

  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");

  
  Serial.println();
  delay(1000);


}
  
