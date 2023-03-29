     void AfterLaunch(){
      fetchGPSData();
       if (millis()-newcalltime<100000){
             tone(buzzer,500);
            newcalltime=millis();
       }
       else{
       noTone(buzzer);
       }
       lastCallTime = millis();
       char * buffer1;
        buffer1= (char*) malloc(70*sizeof(char));
       sprintf(buffer1, "Time: %lu Phase: %d Arm: %f Main: %f alt: %f",lastCallTime, phase, minimumAltitude, minimumMainAltitude, altitude);
       fetchRadio(buffer1);
       free(buffer1);
       //fetchAltimeterData();
       fetchSensorData();//Remove
       Serial.println(altitude);
       Serial.println(minimumAltitude);
       Serial.println(IMU[6]);       
            if (altitude > minimumAltitude) {
                
                noTone(buzzer);
                digitalWrite(6, LOW);
                digitalWrite(7, LOW);
                digitalWrite(8, LOW);
                phase = 3;
              }
           if (analogRead(A18)<=20){
              phase = 1;
              noTone(buzzer);
            }
     }
      
