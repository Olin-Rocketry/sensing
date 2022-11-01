void BeforeApogee(){
  fetchSensorData();
          lastCallTime = millis();
          addDataPoint();
          char *buffer1;
          buffer1= (char*) malloc(70*sizeof(char));
       sprintf(buffer1, "Time: %lu Phase: %d alt: %f",lastCallTime, phase, altitude);
       fetchRadio(buffer1);
       free(buffer1);
          //if the Kalman esitmated velocity goes negative, trigger apogee procedures
<<<<<<< Updated upstream
          if (state[1] < -0.3){//CHANGE BEFORE LUANCH
=======
          if (state[1] < -2){
//          if (altitude > minimumDrogAltitude) {
>>>>>>> Stashed changes
            //trigger the droge e-match fet
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            digitalWrite(8, HIGH);
            digitalWrite(A19, HIGH);
            delay(1000);
            digitalWrite(A19, LOW);
            phase = 4;
            SD_write();
          }
}
