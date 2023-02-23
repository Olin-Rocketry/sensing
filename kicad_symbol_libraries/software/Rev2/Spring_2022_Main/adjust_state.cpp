#include "phoenix_IV_functions.h"

void adjust_state(double *predicted_state, double *measurement, double H[2][3], double kalman_gain[3][2], double *adjusted_state){

    //target equation: x + k(m - Hx)

    //(m - Hx)
    double B[2];

    B[0] = measurement[0] - ((H[0][0] * predicted_state[0]) + (H[0][1] * predicted_state[1]) + (H[0][2] * predicted_state[2]));
    B[1] = measurement[1] - ((H[1][0] * predicted_state[0]) + (H[1][1] * predicted_state[1]) + (H[1][2] * predicted_state[2]));


    adjusted_state[0] = predicted_state[0] + (kalman_gain[0][0] * B[0]) + (kalman_gain[0][1] * B[1]);
    adjusted_state[1] = predicted_state[1] + (kalman_gain[1][0] * B[0]) + (kalman_gain[1][1] * B[1]);
    adjusted_state[2] = predicted_state[2] + (kalman_gain[2][0] * B[0]) + (kalman_gain[2][1] * B[1]);

}
