void SD_write(){
File dataFile = SD.open("flightData.txt", FILE_WRITE); // Not sure about this data type
  if (dataFile) {
    for (int dataPointCount = 0; dataPointCount < batchSize; dataPointCount++) {
        dataFile.write((const uint8_t *)&dataPoints[dataPointCount], sizeof(dataPoint)); 
      }
      Serial.println("------------------------");
      Serial.println("success flightData.txt");
  } else {
    Serial.println("error opening flightData.txt");
  }
  currentDataPoint = 0;
  dataFile.close();
}
