#include "phoenix_IV_functions.h"


void predict_state(double *current_state, double A[3][3], double *predicted_state){
    predicted_state[0] = (A[0][0] * current_state[0]) + (A[0][1] * current_state[1]) + (A[0][2] * current_state[2]);
    predicted_state[1] = (A[1][0] * current_state[0]) + (A[1][1] * current_state[1]) + (A[1][2] * current_state[2]);
    predicted_state[2] = (A[2][0] * current_state[0]) + (A[2][1] * current_state[1]) + (A[2][2] * current_state[2]);
}
