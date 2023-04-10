#include "led_class.h"

Led::Led(byte pin) {
  this->pin = pin;
  init();
}

void Led::init() {
  pinMode(pin, OUTPUT);
  strip = Adafruit_NeoPixel(2, pin, NEO_GRB + NEO_KHZ800);
  strip.begin();
//  off();
}

void Led::green(int pixel) {
  uint32_t c = strip.Color(0, 0, 255);
  strip.setPixelColor(pixel  , c);
  strip.show();
}

void Led::blue(int pixel) {
  uint32_t c = strip.Color(100, 100, 100);
  strip.setPixelColor(pixel  , c);
  strip.show();
}

void Led::off() {
  uint32_t c = strip.Color(0, 0, 0);
  strip.setPixelColor(0  , c);
  strip.setPixelColor(1  , c);
  strip.show();
}
  
