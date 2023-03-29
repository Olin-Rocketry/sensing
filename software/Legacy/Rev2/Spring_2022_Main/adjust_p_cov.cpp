#include "phoenix_IV_functions.h"

void adjust_p_cov(double predicted_p_cov[3][3], double kalman_gain[3][2], double H[2][3], double adjusted_p_cov[3][3]){

    //target equation: p - kHp;


    //Hp
    double B[2][3];

    B[0][0] = (H[0][0] * predicted_p_cov[0][0]) + (H[0][1] * predicted_p_cov[1][0]) + (H[0][2] * predicted_p_cov[2][0]);
    B[0][1] = (H[0][0] * predicted_p_cov[0][1]) + (H[0][1] * predicted_p_cov[1][1]) + (H[0][2] * predicted_p_cov[2][1]);
    B[0][2] = (H[0][0] * predicted_p_cov[0][2]) + (H[0][1] * predicted_p_cov[1][2]) + (H[0][2] * predicted_p_cov[2][2]);

    B[1][0] = (H[1][0] * predicted_p_cov[0][0]) + (H[1][1] * predicted_p_cov[1][0]) + (H[1][2] * predicted_p_cov[2][0]);
    B[1][1] = (H[1][0] * predicted_p_cov[0][1]) + (H[1][1] * predicted_p_cov[1][1]) + (H[1][2] * predicted_p_cov[2][1]);
    B[1][2] = (H[1][0] * predicted_p_cov[0][2]) + (H[1][1] * predicted_p_cov[1][2]) + (H[1][2] * predicted_p_cov[2][2]);

    //p - kHp
    adjusted_p_cov[0][0] = predicted_p_cov[0][0] - ((kalman_gain[0][0] * B[0][0]) + (kalman_gain[0][1] * B[1][0]));
    adjusted_p_cov[0][1] = predicted_p_cov[0][1] - ((kalman_gain[0][0] * B[0][1]) + (kalman_gain[0][1] * B[1][1]));
    adjusted_p_cov[0][2] = predicted_p_cov[0][2] - ((kalman_gain[0][0] * B[0][2]) + (kalman_gain[0][1] * B[1][2]));

    adjusted_p_cov[1][0] = predicted_p_cov[1][0] - ((kalman_gain[1][0] * B[0][0]) + (kalman_gain[1][1] * B[1][0]));
    adjusted_p_cov[1][1] = predicted_p_cov[1][1] - ((kalman_gain[1][0] * B[0][1]) + (kalman_gain[1][1] * B[1][1]));
    adjusted_p_cov[1][2] = predicted_p_cov[1][2] - ((kalman_gain[1][0] * B[0][2]) + (kalman_gain[1][1] * B[1][2]));

    adjusted_p_cov[2][0] = predicted_p_cov[2][0] - ((kalman_gain[2][0] * B[0][0]) + (kalman_gain[2][1] * B[1][0]));
    adjusted_p_cov[2][1] = predicted_p_cov[2][1] - ((kalman_gain[2][0] * B[0][1]) + (kalman_gain[2][1] * B[1][1]));
    adjusted_p_cov[2][2] = predicted_p_cov[2][2] - ((kalman_gain[2][0] * B[0][2]) + (kalman_gain[2][1] * B[1][2]));
}
