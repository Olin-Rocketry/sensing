#include "data_class.h"

Data::Data(Led *statusLed)
{
  this->statusLed = statusLed;
  init();
}

void Data::init()
{
  std::fill_n(packet, packetSize, 3.1415);  //fill packet zeros
  std::fill_n(encodedpacket, packetSize * 4, '0'); //fill encoded packet with zeros
  diagmsg(0);

  //initalize PRISM serial communication

  pinMode(teensyLED, OUTPUT);

  float currentval = diagmsg();
  diagmsg(currentval +  pow(2,8));
}

void Data::SDbegin(bool debugEnable, bool noSD)
{
  this->debugEnable=debugEnable;
  this->noSD = noSD;
  // SD required
  if (!noSD) {
    // see if the card is present and can be initialized:
    if (!SD.begin(10))
    {
      Serial.println("SD Card failed, or not present");
      while (1) { }
    }


   //find the next flight log number
    for (uint8_t i = 0; i < 1000; i++)
    {
      fileName[9] = i / 1000 + '0';
      fileName[10] = i / 100 + '0';
      fileName[11] = i / 10 + '0';
      fileName[12] = i % 10 + '0';
      if (!SD.exists(fileName))
      {
        break;
      }
    }
    //write header to new file
    dataFile = SD.open(fileName, FILE_WRITE);
    dprint("SD opened: "+String(fileName)+"\n");
    dataFile.println(header);
    dataFile.close();
  }
  dprint("Data class started\n");
}


union Data::floatunion_t
{
  float f;
  char a[sizeof(float)];
} float_u;

void Data::bulkencode(float *in, char *out)
{
  for (int i = 0; i < packetSize; i++)
  {
    char encoded[sizeof(float)];
    encoder(encoded, in[i]);
    encoder(encoded, in[i]);
    for (unsigned int j = 0; j < sizeof(float); j++)
    {
      out[i * sizeof(float) + j] = encoded[j];
    }
  }
}

void Data::encoder(char *encoded, float input)
{
  float_u.f = input;
  for (unsigned int i = 0; i < sizeof(float); i++)
  {
    encoded[i] = float_u.a[i];
  }
}

float Data::curtime() {
  return packet[0];
}   void Data::curtime(float i) {
  packet[0] = i;
}
float Data::accelx() {
  return packet[1];
}    void Data::accelx(float i) {
  packet[1] = i;
}
float Data::accely() {
  return packet[2];
}    void Data::accely(float i) {
  packet[2] = i;
}
float Data::accelz() {
  return packet[3];
}    void Data::accelz(float i) {
  packet[3] = i;
}
float Data::gyrox() {
  return packet[4];
}     void Data::gyrox(float i) {
  packet[4] = i;
}
float Data::gyroy() {
  return packet[5];
}     void Data::gyroy(float i) {
  packet[5] = i;
}
float Data::gyroz() {
  return packet[6];
}     void Data::gyroz(float i) {
  packet[6] = i;
}
float Data::temp() {
  return packet[7];
}      void Data::temp(float i) {
  packet[7] = i;
}
float Data::eulerx() {
  return packet[8];
}    void Data::eulerx(float i) {
  packet[8] = i;
}
float Data::eulery() {
  return packet[9];
}    void Data::eulery(float i) {
  packet[9] = i;
}
float Data::eulerz() {
  return packet[10];
}   void Data::eulerz(float i) {
  packet[10] = i;
}
float Data::baralt() {
  return packet[11];
}   void Data::baralt(float i) {
  packet[11] = i;
}
float Data::lng() {
  return packet[12];
}      void Data::lng(float i) {
  packet[12] = i;
}
float Data::lat() {
  return packet[13];
}      void Data::lat(float i) {
  packet[13] = i;
}
float Data::gpsalt() {
  return packet[14];
}   void Data::gpsalt(float i) {
  packet[14] = i;
}
float Data::phs() {
  return packet[15];
}      void Data::phs(float i) {
  packet[15] = i;
}
float Data::cont() {
  return packet[16];
}     void Data::cont(float i) {
  packet[16] = i;
}
float Data::volt() {
  return packet[17];
}     void Data::volt(float i) {
  packet[17] = i;
}
float Data::link() {
  return packet[18];
}     void Data::link(float i) {
  packet[18] = i;
}
float Data::kfx() {
  return packet[19];
}      void Data::kfx(float i) {
  packet[19] = i;
}
float Data::kfy() {
  return packet[20];
}      void Data::kfy(float i) {
  packet[20] = i;
}
float Data::kfz() {
  return packet[21];
}      void Data::kfz(float i) {
  packet[21] = i;
}
float Data::kfvx() {
  return packet[22];
}     void Data::kfvx(float i) {
  packet[22] = i;
}
float Data::kfvy() {
  return packet[23];
}     void Data::kfvy(float i) {
  packet[23] = i;
}
float Data::kfvz() {
  return packet[24];
}     void Data::kfvz(float i) {
  packet[24] = i;
}
float Data::kfdrag() {
  return packet[25];
}   void Data::kfdrag(float i) {
  packet[25] = i;
}
float Data::d() {
  return packet[26];
}        void Data::d(float i) {
  packet[26] = i;
}

// implementing new functions for 28th byte
float Data::diagmsg() {
  return packet[27];
}
void Data::diagmsg(float i) {
  packet[27] = i;
}

void Data::diagmsg_reset() {
  packet[27] = gpsStruct.diagmsg;  
}

 void Data::readGPS(){
//   if (PRISM_serial.available())
//   {
//     PRISM_serial.rxObj(gpsStruct);
//     lat(gpsStruct.lat);
//     lng(gpsStruct.lng);
//     gpsalt(gpsStruct.gpsalt);
//     diagmsg(diagmsg() + gpsStruct.diagmsg);
//   }
 }

void Data::encodeAndAdd()
{
  encodepacket();  //encode the current packet
  addToFrame();  //add packet to frame
}

void Data::encodepacket()
{
  bulkencode(packet, encodedpacket); //endcode the packet
}
 void Data::sendSerialData()
 {

//   digitalWrite(teensyLED, HIGH);
//   PRISM_serial.sendDatum(packet, packetSize*4);
//   digitalWrite(teensyLED, LOW);
 }

void Data::addToFrame()
{
  if (frameIndex % 5 == 0) { //send every 5th packet to EAST
    validpacket=true;
    if(debugEnable==true)
    {
      for(int i=0; i<packetSize; i++)
      {
        Serial.print(String(packet[i])+"\t\t");
      }
      Serial.print("\n");
    }
    
  }

  if (frameIndex >= frameSize) { //when the frame is full, write the frame to SD and clear the frame
    if(!noSD)
    {
      writeSDData();
    }
    dprint(header+"\n");
    frameIndex = 0;  //when done, reset frame location
  }


  //add packet to frame
  for (int index = 0; index < 4 * packetSize; index++) {
    encodedFrame[frameIndex][index] = encodedpacket[index];
  }
  frameIndex++; //increment the location in the frame
}

//write a frame of encoded packets to the SD
void Data::writeSDData()
{
  dataFile = SD.open(fileName, FILE_WRITE); // Not sure about this data type
  if (dataFile)
  {
    for (int batch = 0; batch < frameIndex; batch++) // frameSize is the number of lines in a batch write
    {
      //            char encoded[4*packetSize];
      //            //packetSize is the number of unique numbers to send (27 at the time of writing this)
      //            //since each is a float, you need four bytes/chars for each value in a batch
      //            bulkencode(packet[batch],encoded); //packet is a two-dimensional array that is of size frameSize*packetSize
      //            for (int index=0; index<4*packetSize; index++)
      //            {
      //              dataFile.print(encoded[index]);
      //            }
      for (int index = 0; index < packetSize * 4; index++)
      {
        dataFile.print(encodedFrame[batch][index]);
      }
      dataFile.println();
      //        Serial.print("counter: ");
      //        Serial.println(frameIndex);
      //        Serial.print("batch: ");
      //        Serial.println(encodedFrame[batch]);
    }
  }
  dataFile.close();
  
  dprint("Wrote to SD\n");
}


void Data::analogTelem() {
  
  //=================battery voltage=================


  float raw_analog = analogRead(A8);
  
  volt(map(raw_analog, 815, 856, 3.65, 4.18)); //add to data class

  //==============arming and continuity==============
  int armSignal = analogRead(A8);
  int pyro1Cont = analogRead(A7);
  int pyro2Cont = analogRead(A0);
  int pyroCode = 1000;
  if (armSignal > 100) {
    pyroCode += 1;
  }
  if (pyro1Cont > 100) {
    pyroCode += 10;
  }
  if (pyro2Cont > 100) {
    pyroCode += 100;
  }
  cont(pyroCode);  
}

void Data::dprint(String in)
{
  if(debugEnable==true)
  {
    Serial.print(in);
  }
}
