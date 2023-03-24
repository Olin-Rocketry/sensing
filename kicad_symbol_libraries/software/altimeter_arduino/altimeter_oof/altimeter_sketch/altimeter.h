#ifndef CLASSES
#define CLASSES
#include <Wire.h>
#include <SPI.h>
#include <arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#define SEALEVELPRESSURE_HPA (1013.25)


class Altimeter {

  public:
  Altimeter();
  void init();
  void readAltitude();
  

  private:
  
  Adafruit_BMP3XX bmp;
};

#endif
