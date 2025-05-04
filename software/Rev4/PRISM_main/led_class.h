#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define num_leds 2

class Led {
  private:
    byte pin;
   
    CRGB leds[num_leds];
    
  public:
    Led(byte pin);
    void init();
    void RGB2(int pixel, int R, int G, int B);
    void off();
};
