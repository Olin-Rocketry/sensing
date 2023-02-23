#include "phoenix_IV_functions.h"


void predict_p_cov(double current_p_cov[3][3], double A[3][3], double Q[3][3], double predicted_p_cov[3][3]){
    
    //Target Equation: ApA' + Q

    //(pA')
    double B[3][3];

    B[0][0] = (current_p_cov[0][0] * A[0][0]) + (current_p_cov[0][1] * A[0][1]) + (current_p_cov[0][2] * A[0][2]);
    B[0][1] = (current_p_cov[0][0] * A[1][0]) + (current_p_cov[0][1] * A[1][1]) + (current_p_cov[0][2] * A[1][2]);
    B[0][2] = (current_p_cov[0][0] * A[2][0]) + (current_p_cov[0][1] * A[2][1]) + (current_p_cov[0][2] * A[2][2]);

    B[1][0] = (current_p_cov[1][0] * A[0][0]) + (current_p_cov[1][1] * A[0][1]) + (current_p_cov[1][2] * A[0][2]);
    B[1][1] = (current_p_cov[1][0] * A[1][0]) + (current_p_cov[1][1] * A[1][1]) + (current_p_cov[1][2] * A[1][2]);
    B[1][2] = (current_p_cov[1][0] * A[2][0]) + (current_p_cov[1][1] * A[2][1]) + (current_p_cov[1][2] * A[2][2]);

    B[2][0] = (current_p_cov[2][0] * A[0][0]) + (current_p_cov[2][1] * A[0][1]) + (current_p_cov[2][2] * A[0][2]);
    B[2][1] = (current_p_cov[2][0] * A[1][0]) + (current_p_cov[2][1] * A[1][1]) + (current_p_cov[2][2] * A[1][2]);
    B[2][2] = (current_p_cov[2][0] * A[2][0]) + (current_p_cov[2][1] * A[2][1]) + (current_p_cov[2][2] * A[2][2]);


    //A*(pA') + Q
    predicted_p_cov[0][0] = (A[0][0] * B[0][0]) + (A[0][1] * B[1][0]) + (A[0][2] * B[2][0]) + Q[0][0];
    predicted_p_cov[0][1] = (A[0][0] * B[0][1]) + (A[0][1] * B[1][1]) + (A[0][2] * B[2][1]) + Q[0][1];
    predicted_p_cov[0][2] = (A[0][0] * B[0][2]) + (A[0][1] * B[1][2]) + (A[0][2] * B[2][2]) + Q[0][2];

    predicted_p_cov[1][0] = (A[1][0] * B[0][0]) + (A[1][1] * B[1][0]) + (A[1][2] * B[2][0]) + Q[1][0];
    predicted_p_cov[1][1] = (A[1][0] * B[0][1]) + (A[1][1] * B[1][1]) + (A[1][2] * B[2][1]) + Q[1][1];
    predicted_p_cov[1][2] = (A[1][0] * B[0][2]) + (A[1][1] * B[1][2]) + (A[1][2] * B[2][2]) + Q[1][2];

    predicted_p_cov[2][0] = (A[2][0] * B[0][0]) + (A[2][1] * B[1][0]) + (A[2][2] * B[2][0]) + Q[2][0];
    predicted_p_cov[2][1] = (A[2][0] * B[0][1]) + (A[2][1] * B[1][1]) + (A[2][2] * B[2][1]) + Q[2][1];
    predicted_p_cov[2][2] = (A[2][0] * B[0][2]) + (A[2][1] * B[1][2]) + (A[2][2] * B[2][2]) + Q[2][2];
}
