  #include "altimeter.h"
//
Altimeter::Altimeter() {
  init();

}

void Altimeter::init() {
  Adafruit_BMP3XX bmp;
    Serial.println("Adafruit BMP388 / BMP390 test");

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
}
}
void Altimeter::readAltitude(){
  bmp.performReading();
  Serial.print(bmp.pressure / 100.0);
  
}
  
