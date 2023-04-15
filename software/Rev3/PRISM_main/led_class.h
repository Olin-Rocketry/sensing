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
    void RGB(int pixel, int R, int G, int B);
//    void green(int pixel);
    void off();
};
