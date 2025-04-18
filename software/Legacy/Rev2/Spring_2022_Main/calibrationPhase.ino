void calibrationPhase(){

  // Get Calibration values for system, gyroscope, accelerometer, and magnetometer
  // Calibration values go from 0 to 3, 3 meaning fully calibrated
  // LED lights will activate when gyroscope, accelerometer, and magnetometer are fully calibrated (3 lights)
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  // Displays calibration values
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);

  // LEDs will activate when sensors are calibrated
  if (gyro == 3) {
    digitalWrite(6, HIGH);
  }

  if (accel == 3) {
    digitalWrite(7, HIGH);
  }

  if (mag == 3) {
    digitalWrite(8, HIGH);
  }

  // Switches to pre-launch phase when all sensors are calibrated
  if (system == 3 && gyro == 3 && accel == 3 && mag ==3) {
    phase = 1;
    digitalWrite(8, LOW);
  }
  char * buffer1;
  buffer1= (char*) malloc(70*sizeof(char));
  sprintf(buffer1, "Phase 0: sys:%u G:%u A:%u M:%u\n",system,gyro,accel,mag);
  fetchRadio(buffer1);
  free(buffer1);
  
}
