#include "phoenix_IV_functions.h"

void update_gain(double predicted_p_cov[3][3], double H[2][3], double R[2][2], double kalman_gain[3][2]){

    //target equation: pH' / (HpH' + R)

    double B[3][2], C[2][2], D[2][2];

    //pH'
    B[0][0] = (predicted_p_cov[0][0] * H[0][0]) + (predicted_p_cov[0][1] * H[0][1]) + (predicted_p_cov[0][2] * H[0][2]);
    B[0][1] = (predicted_p_cov[0][0] * H[1][0]) + (predicted_p_cov[0][1] * H[1][1]) + (predicted_p_cov[0][2] * H[1][2]);

    B[1][0] = (predicted_p_cov[1][0] * H[0][0]) + (predicted_p_cov[1][1] * H[0][1]) + (predicted_p_cov[1][2] * H[0][2]);
    B[1][1] = (predicted_p_cov[1][0] * H[1][0]) + (predicted_p_cov[1][1] * H[1][1]) + (predicted_p_cov[1][2] * H[1][2]);

    B[2][0] = (predicted_p_cov[2][0] * H[0][0]) + (predicted_p_cov[2][1] * H[0][1]) + (predicted_p_cov[2][2] * H[0][2]);
    B[2][1] = (predicted_p_cov[2][0] * H[1][0]) + (predicted_p_cov[2][1] * H[1][1]) + (predicted_p_cov[2][2] * H[1][2]);

    //H(pH') + R
    C[0][0] = (H[0][0] * B[0][0]) + (H[0][1] * B[1][0]) + (H[0][2] * B[2][0]) + R[0][0];
    C[0][1] = (H[0][0] * B[0][1]) + (H[0][1] * B[1][1]) + (H[0][2] * B[2][1]) + R[0][1];
    C[1][0] = (H[1][0] * B[0][0]) + (H[1][1] * B[1][0]) + (H[1][2] * B[2][0]) + R[1][0];
    C[1][1] = (H[1][0] * B[0][1]) + (H[1][1] * B[1][1]) + (H[1][2] * B[2][1]) + R[1][1];

    //(HpH' + R)^-1
    double det = (C[0][0] * C[1][1]) - (C[0][1] * C[1][0]);
    D[0][0] = C[1][1] / det;
    D[0][1] = -1 * C[0][1] / det;
    D[1][0] = -1 * C[1][0] / det;
    D[1][1] = C[0][0] / det;


    //pH' * (HpH' =)
    kalman_gain[0][0] = (B[0][0] * D[0][0]) + (B[0][1] * D[1][0]);
    kalman_gain[0][1] = (B[0][0] * D[0][1]) + (B[0][1] * D[1][1]);

    kalman_gain[1][0] = (B[1][0] * D[0][0]) + (B[1][1] * D[1][0]);
    kalman_gain[1][1] = (B[1][0] * D[0][1]) + (B[1][1] * D[1][1]);

    kalman_gain[2][0] = (B[2][0] * D[0][0]) + (B[2][1] * D[1][0]);
    kalman_gain[2][1] = (B[2][0] * D[0][1]) + (B[2][1] * D[1][1]);

}
