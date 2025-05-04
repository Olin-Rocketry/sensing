#include <Arduino.h>
#include <SpeedyStepper.h>
#include <TMCStepper.h>
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define SERIAL_PORT Serial4
#define R_SENSE 0.11f // Match to your driver
class StepperMotor
{

private:
  //pin definitions
  const uint8_t step_pin = 15;
  const uint8_t dir_pin = 20;
  const uint8_t tx_pin = 17;
  const uint8_t rx_pin = 16;

  TMC2209Stepper driver;
  SpeedyStepper stepper;
  int home_speed = 180; //deg/s
  int micro_steps_per_step = 4;
  int full_steps_per_rot = 200;

  const uint8_t run_current_percent = 50;
  const uint8_t stall_guard_threshold = 120;
  int max_speed = 85000; //deg/s
  int max_accel = 2000; //deg/s/s



  //TMC2209 driver object
  //Step Contoller object
  

public:
  StepperMotor();
  void init();
  void enable_stepper();
  void home_stepper();
  void move_stepper(int pos);
  void disable_stepper();
  
};
