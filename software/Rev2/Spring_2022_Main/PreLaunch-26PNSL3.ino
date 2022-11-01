     void PreLaunch(){
        fetchGPSData();
      char * buffer1;
      buffer1= (char*) malloc(70*sizeof(char));
       sprintf(buffer1, "Time: %lu Phase: %d alt: %f",lastCallTime, phase, altitude);
       fetchRadio(buffer1);
       free(buffer1);
            if (analogRead(A18)>=1000){
              int averageAltitude=0;
              int number_readings=5;
              for (int i=0; i<number_readings; i++){
                fetchAltimeterData();
                averageAltitude=altitude+averageAltitude;
                Serial.println(altitude);
              }
<<<<<<< Updated upstream
              minimumAltitude=averageAltitude/number_readings+5;// m //CHANGE BEFORE LUANCH
=======
              Serial.println(altitude);
              minimumAltitude=averageAltitude/number_readings+2;// m //CHANGE BEFORE LUANCH
>>>>>>> Stashed changes
              minimumMainAltitude=averageAltitude/number_readings+5;// m //CHANGE BEFORE LUANCH
              phase = 2;
            }
        }
