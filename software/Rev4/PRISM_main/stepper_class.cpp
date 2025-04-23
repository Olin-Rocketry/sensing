#include "stepper_class.h"


StepperMotor::StepperMotor()
{

    init();
}

void StepperMotor::init()
{
  
}

void StepperMotor::enable_stepper(){
  Serial2.setTX(tx_pin);
  Serial2.setRX(rx_pin);
  step_stick.setup(serial_stream);

  //Step-stick Settings
  step_stick.setRunCurrent(run_current_percent);
  step_stick.setMicrostepsPerStep(8);
  step_stick.moveUsingStepDirInterface();
  step_stick.enableStealthChop();
  step_stick.enableAutomaticCurrentScaling();
  step_stick.enableAutomaticGradientAdaptation();
  step_stick.enableCoolStep();
  step_stick.setStallGuardThreshold(stall_guard_threshold);
  step_stick.enable();

  //Step Driver Settings
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
 // driver.setSpeed(85000);


  }

void StepperMotor::home_stepper(){

  //move at constant speed
  step_stick.moveAtVelocity(home_speed);
  delay(100);  //need delay to prevent stall-trigger on startup current

  //hold untill stall
  while(step_stick.getStallGuardResult() >= stall_guard_threshold){};
  step_stick.moveAtVelocity(0);
  digitalWrite(LED_BUILTIN, HIGH);
  driver.setCurrentPosition(0);
  
  
//  delay(200);
//  digitalWrite(LED_BUILTIN, LOW);
//  //back off slowly
//  step_stick.moveAtVelocity(-40000);
//  delay(1000);
//  //approach limit slowely
//  step_stick.moveAtVelocity(40000);
//  delay(100);
//  while(step_stick.getStallGuardResult() >= stall_guard_threshold){};
//  step_stick.moveAtVelocity(0);
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(1000);
  }

void StepperMotor::move_stepper(int deg_pos){
     driver.moveTo(deg_pos/360.0 * full_steps_per_rot * micro_steps_per_step);
     Serial.print("Target Position:");
     Serial.println(driver.targetPosition());
     Serial.print("Current Position");
     Serial.println(driver.currentPosition());
     driver.runSpeed();
  }

void StepperMotor::disable_stepper(){
  step_stick.disable();
  }
