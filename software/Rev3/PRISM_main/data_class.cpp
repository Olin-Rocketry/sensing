#include "data_class.h"

Data::Data()
{
    Serial.println("Initializing 1");
    init();
}

void Data::init()
{
    Serial.println("Initializing");
    std::fill_n(flightData, dataPointCount, 3.1415926535897932384);
    std::fill_n(encodedFlightData, dataPointCount * 4, '0');
    //  std::fill_n(encodedBatch, batchSize*dataPointCount*4, '0');
    Serial.println("Filename");
    //  for(int i=0; i<1; i++){
    //    strcat(filename,char(rand() % 256));
    //  }
    //  strcat(filename,".txt");
    //  Serial.println("SD");
    //  SD.begin(BUILTIN_SDCARD);
    //  bool ok = SD.sdfs.begin(SdioConfig(DMA_SDIO));
    //  bool ok = SD.sdfs.begin(SdSpiConfig(BUILTIN_SDCARD, DEDICATED_SPI, SD_SCK_MHZ(16)));
    //  if (!ok) {
    //    Serial.println("initialization failed!");
    //    return;
    //  }
    //  if (!SD.begin(BUILTIN_SDCARD)) {
    //    Serial.println("Card failed, or not present");
    //    while (1) {
    //      // No SD card, so don't do anything more - stay stuck here
    //    }
    //  }
    Serial.println("Finished Initializing");
     Serial5.begin(9600);
     SerialTransfer Serial_port; //Create serial port object
 Serial_port.begin(Serial5);
}

void Data::SDbegin()
{
    delay(10);
    Serial.println("Initializing SD card...");
    // see if the card is present and can be initialized:
    if (!SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("Card faild, or not present");
        while (1)
        {
            // No SD card, so don't do anything more - stay stuck here
        }
    }

    Serial.println("card initialized.");

    for (uint8_t i = 0; i < 100; i++)
    {
        fileName[9] = i / 100 + '0';
        fileName[10] = i / 10 + '0';
        fileName[11] = i % 10 + '0';
        if (SD.exists(fileName))
        {
            continue;
        }
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
    for (int i = 0; i < dataPointCount; i++) // sizeof(in)/sizeof(in[0])
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

float Data::curtime() { return flightData[0]; }   void Data::curtime(float i) { flightData[0] = i; }
float Data::accelx() { return flightData[1]; }    void Data::accelx(float i) { flightData[1] = i; }
float Data::accely() { return flightData[2]; }    void Data::accely(float i) { flightData[2] = i; }
float Data::accelz() { return flightData[3]; }    void Data::accelz(float i) { flightData[3] = i; }
float Data::gyrox() { return flightData[4]; }     void Data::gyrox(float i) { flightData[4] = i; }
float Data::gyroy() { return flightData[5]; }     void Data::gyroy(float i) { flightData[5] = i; }
float Data::gyroz() { return flightData[6]; }     void Data::gyroz(float i) { flightData[6] = i; }
float Data::temp() { return flightData[7]; }      void Data::temp(float i) { flightData[7] = i; }
float Data::eulerx() { return flightData[8]; }    void Data::eulerx(float i) { flightData[8] = i; }
float Data::eulery() { return flightData[9]; }    void Data::eulery(float i) { flightData[9] = i; }
float Data::eulerz() { return flightData[10]; }   void Data::eulerz(float i) { flightData[10] = i; }
float Data::baralt() { return flightData[11]; }   void Data::baralt(float i) { flightData[11] = i; }
float Data::lng() { return flightData[12]; }      void Data::lng(float i) { flightData[12] = i; }
float Data::lat() { return flightData[13]; }      void Data::lat(float i) { flightData[13] = i; }
float Data::gpsalt() { return flightData[14]; }   void Data::gpsalt(float i) { flightData[14] = i; }
float Data::phs() { return flightData[15]; }      void Data::phs(float i) { flightData[15] = i; }
float Data::cont() { return flightData[16]; }     void Data::cont(float i) { flightData[16] = i; }
float Data::volt() { return flightData[17]; }     void Data::volt(float i) { flightData[17] = i; }
float Data::link() { return flightData[18]; }     void Data::link(float i) { flightData[18] = i; }
float Data::kfx() { return flightData[19]; }      void Data::kfx(float i) { flightData[19] = i; }
float Data::kfy() { return flightData[20]; }      void Data::kfy(float i) { flightData[20] = i; }
float Data::kfz() { return flightData[21]; }      void Data::kfz(float i) { flightData[21] = i; }
float Data::kfvx() { return flightData[22]; }     void Data::kfvx(float i) { flightData[22] = i; }
float Data::kfvy() { return flightData[23]; }     void Data::kfvy(float i) { flightData[23] = i; }
float Data::kfvz() { return flightData[24]; }     void Data::kfvz(float i) { flightData[24] = i; }
float Data::kfdrag() { return flightData[25]; }   void Data::kfdrag(float i) { flightData[25] = i; }
float Data::d() { return flightData[26]; }        void Data::d(float i) { flightData[26] = i; }

void Data::encodeAndAdd()
{
    encodeFlightData();
    addToBatch();
}

void Data::encodeFlightData()
{
    bulkencode(flightData, encodedFlightData);
}
void Data::sendSerialData()
{
  Serial_port.sendDatum(encodedBatch);
}

void Data::addToBatch()
{
    if (batchCounter >= batchSize)
    {
        writeSDData();
        sendSerialData();
    }
    //  std::fill_n(encodedBatch[batchCounter], dataPointCount*4, '0');
    for (int index = 0; index < 4 * dataPointCount; index++)
    {
        encodedBatch[batchCounter][index] = encodedFlightData[index];
        //    Serial.print(batchCounter);
        //    Serial.print(":");
        //    Serial.print(encodedBatch[batchCounter][index]);
        //    Serial.print(":");
        //    Serial.println(encodedFlightData[index]);
    }
    batchCounter++;
}

void Data::writeSDData()
{
    dataFile = SD.open(fileName, FILE_WRITE); // Not sure about this data type
    if (dataFile)
    {
        for (int batch = 0; batch < batchCounter; batch++) // batchSize is the number of lines in a batch write
        {
            //            char encoded[4*dataPointCount];
            //            //dataPointCount is the number of unique numbers to send (27 at the time of writing this)
            //            //since each is a float, you need four bytes/chars for each value in a batch
            //            bulkencode(flightData[batch],encoded); //flightData is a two-dimensional array that is of size batchSize*dataPointCount
            //            for (int index=0; index<4*dataPointCount; index++)
            //            {
            //              dataFile.print(encoded[index]);
            //            }
            for (int index = 0; index < dataPointCount * 4; index++)
            {
                dataFile.print(encodedBatch[batch][index]);
            }
            dataFile.println();
            //        Serial.print("counter: ");
            //        Serial.println(batchCounter);
            //        Serial.print("batch: ");
            //        Serial.println(encodedBatch[batch]);
        }
    }
    dataFile.close();
    batchCounter = 0;
}