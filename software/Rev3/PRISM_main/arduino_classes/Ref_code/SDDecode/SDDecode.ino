/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;

union floatunion_t {
    float f;
    char a[sizeof (float) ];
} float_u;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // re-open the file for reading:
  myFile = SD.open("testfile.txt");
  if (myFile) {
    Serial.println("File Open");

    // read from the file until there's nothing else in it:
    int counter=0;
    int dataPointCount=28;
    while (myFile.available()) {
      char buf[dataPointCount];
      myFile.readBytesUntil('\n',buf,dataPointCount*4);
      counter++;
      Serial.print("Line ");
      Serial.print(counter);
      Serial.print(": ");
      for(int j=0; j<dataPointCount; j++)
      {
        for(int i=0; i<4; i++)
        {
          char letter=buf[4*j+i];
          float_u.a[i]=letter;
        }
        Serial.print(float_u.f);
        Serial.print(",");
      }
      myFile.readBytesUntil('\n',buf,dataPointCount*4);
      Serial.println();
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}
