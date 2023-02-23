#ifndef CLASSES
#define CLASSES
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class Led {
  
  private:
    byte pin;
    Adafruit_NeoPixel strip;
    
  public:
    Led(byte pin);
    void init();
    void red();
    void green();
    void off();
};
#endif
