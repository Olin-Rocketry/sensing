class Write_SD {

  /*=================
   * Global Variables
   ==================*/

  /*==============
  * Public Methods
  ================*/
  
  public:

    /* Constructor:
     * creates maze with all unvisited locations by
     * initializing maze[][] with all 0s*/
     void SD_writing(String dataString){
             // make a string for assembling the data to log:
            // read three sensors and append to the string:
            // open the file.
            File dataFile = SD.open("datalog.txt", FILE_WRITE);
          
            // if the file is available, write to it:
            if (dataFile) {
              dataFile.println(dataString);
              dataFile.close();
              // print to the serial port too:
              Serial.println(dataString);
            } else {
              // if the file isn't open, pop up an error:
              Serial.println("error opening datalog.txt");
            }
            delay(100); // run at a reasonable not-too-fast speed
     }
};
