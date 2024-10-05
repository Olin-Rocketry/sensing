#pragma once
#include <Arduino.h>
#include <TMC2209.h>
#include <AccelStepper.h>

class StepperMotor
{

private:
  //pin definitions
  const uint8_t step_pin = 15;
  const uint8_t dir_pin = 20;
  const uint8_t tx_pin = 17;
  const uint8_t rx_pin = 16;
  int home_speed = 180; //deg/s
  int micro_steps_per_step = 4;
  int full_steps_per_rot = 200;

  const uint8_t run_current_percent = 20;
  const uint8_t stall_guard_threshold = 120;
  int max_speed = 3600; //deg/s
  int max_accel = 2000; //deg/s/s

  HardwareSerial &serial_stream = Serial2;


  //TMC2209 driver object
  TMC2209 step_stick;
  //Step Contoller object
  AccelStepper driver = AccelStepper(driver.DRIVER, step_pin, dir_pin);
  

public:
  StepperMotor();
  void init();
  void enable_stepper();
  void home_stepper();
  void move_stepper(int pos);
  void disable_stepper();
  
};
