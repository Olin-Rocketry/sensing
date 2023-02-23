#include "phoenix_IV_functions.h"

void kalman_update(double *state, double p_cov[3][3], double *measurement, double dt, double *adjusted_state, double adjusted_p_cov[3][3]){

    double A[3][3] = {{1, dt, dt*dt / 2}, {0, 1, dt}, {0, 0, 1}};     //state transition matrix for position
    double Q[3][3] = {{8, 0, 0}, {0, 4, 0}, {0, 0, 2}};    //model noise covariance
    double H[2][3] = {{1, 0, 0}, {0, 0, 1}};     //measurement jacobian (to transforms the system state into the measurement)
    double R[2][2] = {{10, 0}, {0, 2}};  //measurement noise covariance
    double kalman_gain[3][2];


    predict_state(state, A, state);
    predict_p_cov(p_cov, A, Q, p_cov);
    update_gain(p_cov, H, R, kalman_gain);
    adjust_state(state, measurement, H, kalman_gain, adjusted_state);
    adjust_p_cov(p_cov, kalman_gain, H, adjusted_p_cov);
}
