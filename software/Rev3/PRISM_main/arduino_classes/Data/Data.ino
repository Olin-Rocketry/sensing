#include "data_class.h"

Data test;
float timestep=0.0;

void setup() {
  Serial.begin(9600); 
  Serial.println("Starting");
}
void loop() {
  Serial.println(timestep);
  test.curtime(timestep);
  Serial.println(test.curtime());
  test.encodeAndAdd();
  test.writeSDData();
  delay(1000);
  timestep+=1;
}
