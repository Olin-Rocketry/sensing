#pragma once
#include <Arduino.h>
#include "data_class.h"

class Kalman {
  private:


    
    double A[3][3];     //state transition matrix for position
    double Q[3][3];    //model noise covariance
    double H[2][3];     //measurement jacobian (to transforms the system state into the measurement)
    double R[2][2];  //measurement noise covariance  this is what we can change
    double predicted_state[3];
    double adjusted_state[3];
    double current_p_cov[3][3];     //process covarience (main state uncertainty)
    double predicted_p_cov[3][3];
    double adjusted_p_cov[3][3];
    double kalman_gain[3][2];

    Data *data;

    unsigned long old_time;
    unsigned long curr_time;
    double dt;

    //kalman functions
    void predict_state();
    void predict_p_cov();
    void update_gain();
    void adjust_p_cov();
    void adjust_state();
    void init();



  public:
    double measurement[2];
    double current_state[3];

    Kalman(Data *data);   //constructor
    void update();   //main kalman filter caller
    void begin();
};
