#include "led_class.h"

Led::Led(byte pin) {
  this->pin = pin;
}

void Led::init() {
  FastLED.addLeds<WS2811, 0, GRB>(leds, num_leds);
//  off();
}

void Led::RGB(int pixel, int R, int G, int B) {
  leds[pixel] = CRGB(R,G,B);
  FastLED.show();
}


void Led::off() {
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  FastLED.show();
}
  
