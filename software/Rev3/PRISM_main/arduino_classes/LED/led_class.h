#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class Led {
  private:
    byte pin;
    Adafruit_NeoPixel strip;
    
  public:
    Led(byte pin);
    void init();
    void blue(int pixel);
    void green(int pixel);
    void off();
};
