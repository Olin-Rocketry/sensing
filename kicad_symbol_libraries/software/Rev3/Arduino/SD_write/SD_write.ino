void SD_write(){
File dataFile = SD.open("flightData.txt", FILE_WRITE); // Not sure about this data type
  if (dataFile) {
      for (int batch=0; batch<batchSize; batch++) //batchSize is the number of lines in a batch write
      {
        char encoded[4*dataPointCount];
        //dataPointCount is the number of unique numbers to send (27 at the time of writing this)
        //since each is a float, you need four bytes/chars for each value in a batch
        bulkencode(flightData[batch],encoded); //flightData is a two-dimensional array that is of size batchSize*dataPointCount
        for (int index=0; index<4*dataPointCount; index++)
        {
          dataFile.print(encoded[index]);
        }
        dataFile.println();
      }
//      Serial.println("------------------------");
//      Serial.println("success flightData.txt");
  } else {
//    Serial.println("error opening flightData.txt");
  }
  dataFile.close();
}
