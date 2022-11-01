void addDataPoint()
{
      if (currentDataPoint == batchSize)
      {
            SD_write();
            currentDataPoint = 0;
      }
      dataPoints[currentDataPoint].timeElapsed = millis();

      dataPoints[currentDataPoint].IMU[0] = IMU[0];
      dataPoints[currentDataPoint].IMU[1] = IMU[1];
      dataPoints[currentDataPoint].IMU[2] = IMU[2];
      dataPoints[currentDataPoint].IMU[3] = IMU[3];
      dataPoints[currentDataPoint].IMU[4] = IMU[4];
      dataPoints[currentDataPoint].IMU[5] = IMU[5];
      dataPoints[currentDataPoint].IMU[6] = IMU[6];

      dataPoints[currentDataPoint].GPSArray[0] = GPSArray[0];
      dataPoints[currentDataPoint].GPSArray[1] = GPSArray[1];

      dataPoints[currentDataPoint].altitude = altitude;
      dataPoints[currentDataPoint].phase = phase;

      if (firstKalman)
      {
            dataPoints[currentDataPoint].kalmanState[0] = altitude; // alt
            //MUST CHANGE BEFORE LUANCH
            //dataPoints[currentDataPoint].kalmanState[1] = 30;        // vel
            dataPoints[currentDataPoint].kalmanState[1] = 3;        // vel
            dataPoints[currentDataPoint].kalmanState[2] = IMU[6];   // acc

            // fill inital state vector
            state[0] = altitude;
             //MUST CHANGE BEFORE LUANCH
            //state[1] = 30;      // assume intial velocity is zero
            state[1] = 3;      // assume intial velocity is zero
            state[2] = IMU[6]; // global z
            firstKalman = false;
      }
      else
      {
            double dt = ((double)dataPoints[currentDataPoint].timeElapsed - dataPoints[currentDataPoint - 1].timeElapsed) / 1000; // needs to be in seconds
            double measurement[2] = {altitude, IMU[6]};
            kalman_update(state, p_cov, measurement, dt, state, p_cov);
            dataPoints[currentDataPoint].kalmanState[0] = state[0];
            dataPoints[currentDataPoint].kalmanState[1] = state[1];
            dataPoints[currentDataPoint].kalmanState[2] = state[2];
      }


      //Serial.print(dataPoints[currentDataPoint].altitude);
      //Serial.print("\t");
     
      //Serial.print(dataPoints[currentDataPoint].kalmanState[0]);
      //Serial.print("\t");
      Serial.print(dataPoints[currentDataPoint].kalmanState[1]);
      Serial.print("\t");
      Serial.println(dataPoints[currentDataPoint].kalmanState[2]);
       currentDataPoint += 1;
}
