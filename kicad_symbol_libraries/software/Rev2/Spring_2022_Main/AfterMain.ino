 void AfterMain(){
          fetchSensorData();
          addDataPoint();
          char *buffer1;
          buffer1= (char*) malloc(70*sizeof(char));
       sprintf(buffer1, "Time: %lu Phase: %d alt: %f",lastCallTime, phase, altitude);
       fetchRadio(buffer1);
       free(buffer1);
          lastCallTime = millis();
}
