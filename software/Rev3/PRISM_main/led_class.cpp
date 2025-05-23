#include "led_class.h"

Led::Led(byte pin) {
  this->pin = pin;
  init();
}

void Led::init() {
  tone(33, 600, 1000);

  FastLED.addLeds<WS2811, 34, GRB>(leds, num_leds);
//  off();
}

void Led::RGB(int pixel, int R, int G, int B) {
  leds[pixel] = CRGB(R,G,B);;
    FastLED.show();
}


void Led::off() {
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  FastLED.show();
}
  
