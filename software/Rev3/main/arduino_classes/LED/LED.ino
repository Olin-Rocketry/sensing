#include "led_class.h"


Led led1(23);


void setup() { 
}
void loop() {
    led1.red();
    delay(1000);
    led1.green();
    delay(1000);
    led1.off();
    delay(1000);
  }
