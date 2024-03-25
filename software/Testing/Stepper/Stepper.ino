
#include "stepper_class.h"

#include <Arduino.h>



StepperMotor steppermotor;


void setup()
{
    steppermotor.enable_stepper();
     
  
  Serial.begin(115200);

  steppermotor.home_stepper();


}

void loop()
{




  
  // stepper
  int t_now = millis();

  int pos = t_now/1000 * 360;

  Serial.println(pos);
  
  steppermotor.move_stepper(pos);

  if (t_now > 1000*30){
    steppermotor.disable_stepper();
  }
    
 
}
