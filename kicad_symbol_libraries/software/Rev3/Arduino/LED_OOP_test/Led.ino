#include "classes.h"

Led::Led(byte pin) {
  this->pin = pin;
  init();
}

void Led::init() {
  pinMode(pin, OUTPUT);
  strip = Adafruit_NeoPixel(2, pin, NEO_GRB + NEO_KHZ800);
  strip.begin();
  off();
}

void Led::green() {
  uint32_t c = strip.Color(0, 255, 0);
  strip.setPixelColor(0  , c);
 
  strip.show();
}

void Led::red() {
  uint32_t c = strip.Color(255, 0, 0);
  strip.setPixelColor(0  , c);

  strip.show();
}

void Led::off() {
  uint32_t c = strip.Color(0, 0, 0);
  strip.setPixelColor(0  , c);
  strip.show();
}
  
