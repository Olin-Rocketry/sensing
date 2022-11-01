 void BeforeMain(){
          fetchSensorData();
          lastCallTime = millis();
          addDataPoint();
           char *buffer1;
          buffer1= (char*) malloc(70*sizeof(char));
      sprintf(buffer1, "Time: %lu Phase: %d alt: %f",lastCallTime, phase, altitude);
       fetchRadio(buffer1);
       free(buffer1);
          if (altitude < minimumMainAltitude) {


            

            
            digitalWrite(A20, HIGH);
            delay(1000);
            digitalWrite(A20, LOW);

            
            phase = 5;
            SD_write();
          }
          }
