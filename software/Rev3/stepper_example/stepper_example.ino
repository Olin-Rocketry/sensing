#include "stepper_class.h"
#include <Arduino.h>



StepperMotor steppermotor;




void setup(){

  steppermotor.enable_stepper();
//  steppermotor.home_stepper();

  Serial.begin(115200);

  
}


void loop(){
  int t_now = millis();

  int pos = t_now/10000 * 360*20;

  Serial.println(pos);
  
  steppermotor.move_stepper(pos);


  if (t_now > 1000*30){
    steppermotor.disable_stepper();
  }
  
}
