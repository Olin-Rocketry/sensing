#include "sd_class.h"


Sd sd1(BUILTIN_SDCARD);


void setup() { 
  Serial.begin(9600);
  while (!Serial) {
  }
  
  sd1.begin();
}


void loop() {

  String line = "hello";


  sd1.write_sd(line);

  delay(100); // run at a reasonable not-too-fast speed

  }
