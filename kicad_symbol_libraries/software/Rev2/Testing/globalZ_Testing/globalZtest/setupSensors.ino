 void setupSensors(){
  Serial.begin(115200);
  while(!Serial){
    Serial.println("Serial not working");
  }
  
  // Configure Sensors

  // IMU
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1);
  }
   bno.setExtCrystalUse(true);
  
  
  // Configure LED pins
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

   

 }
