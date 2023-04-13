#include "data_class.h"

Data::Data()
{
    Serial.println("Initializing 1");
    init();
}

void Data::init()
{
    Serial.println("Initializing");
    std::fill_n(packet, packetSize, 0.0000000);  //fill packet zeros
    std::fill_n(encodedpacket, packetSize * 4, '0'); //fill encoded packet with zeros
    //  std::fill_n(encodedFrame, frameSize*packetSize*4, '0');
    


    //initalize PRISM serial communication
    Serial5.begin(115200);
    PRISM_rx.begin(Serial5);
    PRISM_tx.begin(Serial5);
    Serial.println("Serial-5 Opend");
}

void Data::SDbegin()
{
    //initialize the SD card for writing
    delay(10);
    Serial.println("Initializing SD card...");
    // see if the card is present and can be initialized:
    if (!SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("Card faild, or not present");
        while (1) { }
    }

    Serial.println("card initialized.");

    //find the next flight log number
    for (uint8_t i = 0; i < 100; i++)
    {
        fileName[9] = i / 100 + '0';
        fileName[10] = i / 10 + '0';
        fileName[11] = i % 10 + '0';
        if (SD.exists(fileName))
        {
            continue;
        }
        //write header to new file
        Serial.print("Writing to: ");
        Serial.println(fileName);
        dataFile = SD.open(fileName, FILE_WRITE);
        dataFile.println("Time, Phase, Alt, AccX");
        dataFile.close();
        break;
    }
}

union Data::floatunion_t
{
    float f;
    char a[sizeof(float)];
} float_u;

void Data::bulkencode(float *in, char *out)
{
    // in and out should be char arrays of the same size
    for (int i = 0; i < packetSize; i++) // sizeof(in)/sizeof(in[0])
    {
        // Serial.print("encoding: ");
        // Serial.println(sizeof(in)/sizeof(in[0]));
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

float Data::curtime() { return packet[0]; }   void Data::curtime(float i) { packet[0] = i; }
float Data::accelx() { return packet[1]; }    void Data::accelx(float i) { packet[1] = i; }
float Data::accely() { return packet[2]; }    void Data::accely(float i) { packet[2] = i; }
float Data::accelz() { return packet[3]; }    void Data::accelz(float i) { packet[3] = i; }
float Data::gyrox() { return packet[4]; }     void Data::gyrox(float i) { packet[4] = i; }
float Data::gyroy() { return packet[5]; }     void Data::gyroy(float i) { packet[5] = i; }
float Data::gyroz() { return packet[6]; }     void Data::gyroz(float i) { packet[6] = i; }
float Data::temp() { return packet[7]; }      void Data::temp(float i) { packet[7] = i; }
float Data::eulerx() { return packet[8]; }    void Data::eulerx(float i) { packet[8] = i; }
float Data::eulery() { return packet[9]; }    void Data::eulery(float i) { packet[9] = i; }
float Data::eulerz() { return packet[10]; }   void Data::eulerz(float i) { packet[10] = i; }
float Data::baralt() { return packet[11]; }   void Data::baralt(float i) { packet[11] = i; }
float Data::lng() { return packet[12]; }      void Data::lng(float i) { packet[12] = i; }
float Data::lat() { return packet[13]; }      void Data::lat(float i) { packet[13] = i; }
float Data::gpsalt() { return packet[14]; }   void Data::gpsalt(float i) { packet[14] = i; }
float Data::phs() { return packet[15]; }      void Data::phs(float i) { packet[15] = i; }
float Data::cont() { return packet[16]; }     void Data::cont(float i) { packet[16] = i; }
float Data::volt() { return packet[17]; }     void Data::volt(float i) { packet[17] = i; }
float Data::link() { return packet[18]; }     void Data::link(float i) { packet[18] = i; }
float Data::kfx() { return packet[19]; }      void Data::kfx(float i) { packet[19] = i; }
float Data::kfy() { return packet[20]; }      void Data::kfy(float i) { packet[20] = i; }
float Data::kfz() { return packet[21]; }      void Data::kfz(float i) { packet[21] = i; }
float Data::kfvx() { return packet[22]; }     void Data::kfvx(float i) { packet[22] = i; }
float Data::kfvy() { return packet[23]; }     void Data::kfvy(float i) { packet[23] = i; }
float Data::kfvz() { return packet[24]; }     void Data::kfvz(float i) { packet[24] = i; }
float Data::kfdrag() { return packet[25]; }   void Data::kfdrag(float i) { packet[25] = i; }
float Data::d() { return packet[26]; }        void Data::d(float i) { packet[26] = i; }

void Data::readGPS()
{
  if(PRISM_rx.available())
  {
    PRISM_rx.rxObj(gpsStruct);
    lat(gpsStruct.lat);
    lng(gpsStruct.lng);
    gpsalt(gpsStruct.gpsalt);
  }
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
//  PRISM_tx.sendDatum(encodedFrame[frameSize-1]);
// 27 chars: 111111111111111111111111111
  PRISM_tx.sendDatum("111111111111111111111111111111111111111111111111111111");
}

void Data::addToFrame()
{
    if (frameIndex % 10 == 0){  //send every 10th packet to EAST
        sendSerialData();
    }
    
    if (frameIndex >= frameSize){  //when the frame is full, write the frame to SD and clear the frame
        writeSDData();
    }
    //  std::fill_n(encodedFrame[frameIndex], packetSize*4, '0');

    //add packet to frame
    for (int index = 0; index < 4 * packetSize; index++){
        encodedFrame[frameIndex][index] = encodedpacket[index];
        //    Serial.print(frameIndex);
        //    Serial.print(":");
        //    Serial.print(encodedFrame[frameIndex][index]);
        //    Serial.print(":");
        //    Serial.println(encodedpacket[index]);
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
    frameIndex = 0;  //when done, reset frame location
}
