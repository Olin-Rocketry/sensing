 void setupSensors(){

  // Configure Sensors
  
  //Setup SD CARD
 // const int chipSelect = 4;
  // see if the card is present and can be initialized:
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  else{
  Serial.println("Card initialized.");
  }
  
  // IMU
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1);
  }
   bno.setExtCrystalUse(true);
  //Altimeter
  if (!mpl.begin()) {
    Serial.println("NO MPL3115A2 detected");
    while(1);
  }
  mpl.setSeaPressure(1013.26);
  
  // Configure LED pins
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

   

  //GPS
  GPS.begin(9600);  // 9600 baud is the default rate for the Ultimate GPS
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate


//Radio
    pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.println("Arduino LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);

//Buzzer
 pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
 }
