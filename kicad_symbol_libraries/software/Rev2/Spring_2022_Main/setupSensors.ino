 void setupSensors(){
if(IIC_Read(0x0C) != 196){ //checks whether sensor is readable (who_am_i bit)
  Serial.println("i2c bad");
}
  IIC_Write(0x2D,0); //write altitude offset=0 (because calculation below is based on offset=0)
  //calculate sea level pressure by averaging a few readings
  Serial.println("Pressure calibration...");
  float buff[4];
  for (byte i=0;i<4;i++){
    IIC_Write(0x26, 0b00111011); //bit 2 is one shot mode, bits 4-6 are 128x oversampling
    IIC_Write(0x26, 0b00111001); //must clear oversampling (OST) bit, otherwise update will be once per second
    delay(550); //wait for sensor to read pressure (512ms in datasheet)
    IIC_ReadData(); //read sensor data
    buff[i] = Baro_Read(); //read pressure
    Serial.println(buff[i]);
  }
  float currpress=(buff[0]+buff[1]+buff[2]+buff[3])/4; //average over two seconds
 
  Serial.print("Current pressure: "); Serial.print(currpress); Serial.println(" Pa");
  //calculate pressure at mean sea level based on a given altitude
  float seapress = currpress/pow(1-ALTBASIS*0.0000225577,5.255877);
  Serial.print("Sea level pressure: "); Serial.print(seapress); Serial.println(" Pa");
  Serial.print("Temperature: ");
  Serial.print(IICdata[3]+(float)(IICdata[4]>>4)/16); Serial.println(" C");
 
  // This configuration option calibrates the sensor according to
  // the sea level pressure for the measurement location (2 Pa per LSB)
  IIC_Write(0x14, (unsigned int)(seapress / 2)>>8);//IIC_Write(0x14, 0xC3); // BAR_IN_MSB (register 0x14):
  IIC_Write(0x15, (unsigned int)(seapress / 2)&0xFF);//IIC_Write(0x15, 0xF3); // BAR_IN_LSB (register 0x15):
  // Configure Sensors
  
 
  // IMU
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1);
  }
   bno.setExtCrystalUse(true);
  //Altimeter
//  if (!mpl.begin()) {
//    Serial.println("NO MPL3115A2 detected");
//    while(1);
//  }
//  mpl.setSeaPressure(1013.26);
  
  // Configure LED pins
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  

 
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
pinMode(buzzer, OUTPUT);


 //Setup SD CARD
 // const int chipSelect = 4;
  // see if the card is present and can be initialized:
  char ch[70];
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  else{
    Serial.println("Card initialized.");
    File dataFile = SD.open("flightData.txt", FILE_WRITE); // Not sure about this data type
    if (dataFile) {
           dataFile.println("Start");
     }
     dataFile.close(); // close the file
     File data1 = SD.open("flightData.txt"); // Not sure about this data type
     if (data1) {
          
          int count=0;
          while (data1.available()) {
               ch[count] = data1.read();
               count++;
   
          }
      }
   data1.close(); // Close the file
   char * buffer1;
   buffer1= (char*) malloc(70*sizeof(char));
   strcpy(buffer1, ch);
   fetchRadio(buffer1);
   free(buffer1);
 }
 }
