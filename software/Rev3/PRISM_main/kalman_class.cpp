#include "kalman_class.h"

Kalman::Kalman(Data *data) {
  this->data = data;
  init();
}

void Kalman::init() {
 
  Q[0][0] = 8; Q[0][1] = 0; Q[0][2] = 0;
  Q[1][0] = 0; Q[1][1] = 4; Q[1][2] = 0;
  Q[2][0] = 0; Q[2][1] = 0; Q[2][2] = 2;

  H[0][0] = 1; H[0][1] = 0; H[0][2] = 0;
  H[1][0] = 0; H[1][1] = 0; H[1][2] = 1;

  R[0][0] = 10; R[1][0] = 0;
  R[1][0] = 0;  R[1][1] = 2;

  current_p_cov[0][0] = 3; current_p_cov[0][1] = 0; current_p_cov[0][2] = 0;
  current_p_cov[1][0] = 0; current_p_cov[1][1] = 2; current_p_cov[1][2] = 0;
  current_p_cov[2][0] = 0; current_p_cov[2][1] = 0; current_p_cov[2][2] = 1;
}

void Kalman::begin(){
  old_time = millis();

  current_state[0] = data->baralt(); // alt
  current_state[1] = 3;        // vel    MUST CHANGE BEFORE LUANCH!!!!! : expected launch velocity
  current_state[2] = data->accelz();  //acceleration
}

void Kalman::update() {
  //run through all the steps to update the current kalman filter estimate of the state

  //find the timestep dt
  curr_time = millis();
  dt = (double)((curr_time - old_time) / 1000);

  

  A[0][0] = 1; A[0][1] = dt;  A[0][2] = dt * dt / 2;
  A[1][0] = 0; A[1][1] = 1; A[1][2] = dt;
  A[2][0] = 0; A[2][1] = 0; A[2][2] = 1;


  predict_state();
  predict_p_cov();
  update_gain();
  adjust_state();
  adjust_p_cov();

  //update variables
  memcpy(current_state, adjusted_state, sizeof(adjusted_state));
  memcpy(current_p_cov, adjusted_p_cov, sizeof(adjusted_p_cov));

  data->kfz(current_state[0]);
  data->kfvz(current_state[1]);

}

void Kalman::predict_state() {
  //state model: uses simple equations of motion to predict the future state of the vehicle given the current acceleration.
  predicted_state[0] = (A[0][0] * current_state[0]) + (A[0][1] * current_state[1]) + (A[0][2] * current_state[2]);
  predicted_state[1] = (A[1][0] * current_state[0]) + (A[1][1] * current_state[1]) + (A[1][2] * current_state[2]);
  predicted_state[2] = (A[2][0] * current_state[0]) + (A[2][1] * current_state[1]) + (A[2][2] * current_state[2]);
}

void Kalman::predict_p_cov() {

  //Used to predicit the error/uncertainty in the state model
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

void Kalman::update_gain() {
  //update gain based on measurement noise and model noise
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

void Kalman::adjust_state() {
  //update state with gain and new measurement
  //target equation: x + k(m - Hx)

  //(m - Hx)
  double B[2];

  B[0] = measurement[0] - ((H[0][0] * predicted_state[0]) + (H[0][1] * predicted_state[1]) + (H[0][2] * predicted_state[2]));
  B[1] = measurement[1] - ((H[1][0] * predicted_state[0]) + (H[1][1] * predicted_state[1]) + (H[1][2] * predicted_state[2]));


  adjusted_state[0] = predicted_state[0] + (kalman_gain[0][0] * B[0]) + (kalman_gain[0][1] * B[1]);
  adjusted_state[1] = predicted_state[1] + (kalman_gain[1][0] * B[0]) + (kalman_gain[1][1] * B[1]);
  adjusted_state[2] = predicted_state[2] + (kalman_gain[2][0] * B[0]) + (kalman_gain[2][1] * B[1]);

}

void Kalman::adjust_p_cov() {

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
