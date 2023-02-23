float sensor_read_data(){
  // This function reads the altitude (or barometer) and temperature registers, then prints their values
  // variables for the calculations
  float altbaro;
 
  //One shot mode at 0b10101011 is slightly too fast, but better than wasting sensor cycles that increase precision
  //one reading seems to take 4ms (datasheet p.33);
  //oversampling at 32x=130ms interval between readings seems to be optimal for 10Hz
//  #ifdef ALTMODE //Altitude mode
    IIC_Write(0x26, 0b10101011); //bit 2 is one shot mode //0xB9 = 0b10111001
    IIC_Write(0x26, 0b10101001); //must clear oversampling (OST) bit, otherwise update will be once per second
//  #else //Barometer mode
//    IIC_Write(0x26, 0b00111011); //bit 2 is one shot mode //0xB9 = 0b10111001
//    IIC_Write(0x26, 0b00111001); //must clear oversampling (OST) bit, otherwise update will be once per second
//  #endif
//  delay(100); //read with 10Hz; drop this if calling from an outer loop
 
  IIC_ReadData(); //reads registers from the sensor
//  m_temp = IICdata[3]; //temperature, degrees
//  l_temp = (float)(IICdata[4]>>4)/16.0; //temperature, fraction of a degree
//  temperature = (float)(m_temp + l_temp);
 
//  #ifdef ALTMODE //converts byte data into float; change function to Alt_Read() or Baro_Read()
    altbaro = Alt_Read();
    return altbaro;
//  #else
//    altbaro = Baro_Read();
//  #endif
}
 
float Baro_Read(){
  //this function takes values from the read buffer and converts them to pressure units
  unsigned long m_altitude = IICdata[0];
  unsigned long c_altitude = IICdata[1];
  float l_altitude = (float)(IICdata[2]>>4)/4; //dividing by 4, since two lowest bits are fractional value
  return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); //shifting 2 to the left to make room for LSB
}
 
float Alt_Read(){
  //Reads altitude data (if CTRL_REG1 is set to altitude mode)
  int m_altitude = IICdata[0];
  int c_altitude = IICdata[1];
  float l_altitude = (float)(IICdata[2]>>4)/16;
  return((float)((m_altitude << 8)|c_altitude) + l_altitude);
}
 
byte IIC_Read(byte regAddr){
  // This function reads one byte over I2C
  Wire.beginTransmission(SENSORADDRESS);
  Wire.write(regAddr); // Address of CTRL_REG1
  Wire.endTransmission(false); // Send data to I2C dev with option for a repeated start. Works in Arduino V1.0.1
  Wire.requestFrom(SENSORADDRESS, 1);
  return Wire.read();
}
 
void IIC_ReadData(){  //Read Altitude/Barometer and Temperature data (5 bytes)
  //This is faster than reading individual register, as the sensor automatically increments the register address,
  //so we just keep reading...
  byte i=0;
  Wire.beginTransmission(SENSORADDRESS);
  Wire.write(0x01); // Address of CTRL_REG1
  Wire.endTransmission(false);
  Wire.requestFrom(SENSORADDRESS,5); //read 5 bytes: 3 for altitude or pressure, 2 for temperature
  while(Wire.available()) IICdata[i++] = Wire.read();
}
 
void IIC_Write(byte regAddr, byte value){
  // This function writes one byto over I2C
  Wire.beginTransmission(SENSORADDRESS);
  Wire.write(regAddr);
  Wire.write(value);
  Wire.endTransmission(true);
}
