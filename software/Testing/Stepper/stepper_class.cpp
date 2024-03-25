#include "stepper_class.h"


StepperMotor::StepperMotor()
{

    init();
}

void StepperMotor::init()
{
  
}

void StepperMotor::enable_stepper(){
  Serial4.setTX(tx_pin);
  Serial4.setRX(rx_pin);
  step_stick.setup(serial_stream);

  //Step-stick Settings
  step_stick.setRunCurrent(run_current_percent);
  step_stick.setMicrostepsPerStep(micro_steps_per_step);
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

  driver.setMaxSpeed(max_speed/360*full_steps_per_rot*micro_steps_per_step);
  driver.setAcceleration(max_accel/360*full_steps_per_rot*micro_steps_per_step);
  
  }

void StepperMotor::home_stepper(){

  //move at constant speed
  step_stick.moveAtVelocity(home_speed);
  delay(1000);  //need delay to prevent stall-trigger on startup current

  //hold untill stall
  double stall = step_stick.getStallGuardResult();
  while(stall >= stall_guard_threshold){
    Serial.println(stall);
    stall = step_stick.getStallGuardResult();
    delay(5);
  };
  
  step_stick.moveAtVelocity(0);
  digitalWrite(LED_BUILTIN, HIGH);
}
  

 
void StepperMotor::move_stepper(int deg_pos){
     driver.moveTo(deg_pos/360.0 * full_steps_per_rot * micro_steps_per_step);
     driver.run();


  }

void StepperMotor::disable_stepper(){
  step_stick.disable();
  }
