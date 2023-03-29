void fetchGPSData() {
  // read data from the GPS in the 'main loop'

    if (GPS.fix) {
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      GPSArray[0]=GPS.latitude;
      GPSArray[1]=GPS.longitude;
    }
    else{ 
      GPSArray[0]=3.;
      GPSArray[1]=3.;
    }
}
  
