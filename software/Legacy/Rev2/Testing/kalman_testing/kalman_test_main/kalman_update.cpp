#include "phoenix_IV_functions.h"
#include <Arduino.h>

void kalman_update(double *state, double p_cov[3][3], double *measurement, double dt, double *adjusted_state, double adjusted_p_cov[3][3]){

    double A[3][3] = {{1, dt, dt*dt / 2}, {0, 1, dt}, {0, 0, 1}};     //state transition matrix for position
    //Serial.print("time \t");
    //Serial.println(dt);
    double Q[3][3] = {{8, 0, 0}, {0, 4, 0}, {0, 0, 2}};    //model noise covariance
    double H[2][3] = {{1, 0, 0}, {0, 0, 1}};     //measurement jacobian (to transforms the system state into the measurement)
    double R[2][2] = {{10, 0}, {0, 2}};  //measurement noise covariance
    double kalman_gain[3][2];


    predict_state(state, A, state);

    
//    Serial.print("Predicted State: \t");
//    Serial.print(state[0]);
//    Serial.print("\t");
//    Serial.print(state[1]);
//    Serial.print("\t");
//    Serial.println(state[2]);

    
    predict_p_cov(p_cov, A, Q, p_cov);
    update_gain(p_cov, H, R, kalman_gain);
    adjust_state(state, measurement, H, kalman_gain, adjusted_state);

    
//    Serial.print("Adjusted State: \t");
//    Serial.print(adjusted_state[0]);
//    Serial.print("\t");
//    Serial.print(adjusted_state[1]);
//    Serial.print("\t");
//    Serial.println(adjusted_state[2]);

    
    adjust_p_cov(p_cov, kalman_gain, H, adjusted_p_cov);
}
