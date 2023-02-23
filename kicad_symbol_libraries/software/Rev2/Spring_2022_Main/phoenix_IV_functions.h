///////////////////////////////////////////////////////////////////////////
//
//  Description:  
//      Flight controll library for Phoenix IV
//
//  Comments:
//
//  Rowan Jansens
//  Olin College of Engineering
//  1/23/2022
///////////////////////////////////////////////////////////////////////////    


#ifndef phoenix_IV_functions_h
#define phoenix_IV_functions_h


#include <math.h>
#include <stdio.h>

//comment this line for cpp testing but uncomment for arduino
//#include <Arduino.h>

//kalman function declerations
void predict_state(double *current_state, double A[3][3], double *predicted_state);
void predict_p_cov(double current_p_cov[3][3], double A[3][3], double Q[3][3], double predicted_p_cov[3][3]);
void update_gain(double predicted_p_cov[3][3], double H[2][3], double R[2][2], double kalman_gain[3][2]);
void adjust_state(double *predicted_state, double *measurement, double H[2][3], double kalman_gain[3][2], double *adjusted_state);
void adjust_p_cov(double predicted_p_cov[3][3], double kalman_gain[3][2], double H[2][3], double adjusted_p_cov[3][3]);
void kalman_update(double *state, double p_cov[3][3], double *measurement, double dt, double *adjusted_state, double adjusted_p_cov[3][3]);


#endif
