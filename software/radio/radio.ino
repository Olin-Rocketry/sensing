#include <iostream>
#include <string>
using namespace std;
#include <SPI.h>
#include "radio_class.h"
Radio radio;

#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCLK   14
#define HSPI_SS     16
#define OE          21

uint8_t numbercodes[11] = {
    // Reversed bits for each original byte
    0b11101011, // 0
    0b00101000, // 1
    0b10110011, // 2
    0b10111010, // 3
    0b01111000, // 4
    0b11011010, // 5
    0b11011011, // 6
    0b10101000, // 7
    0b11111011, // 8
    0b11111010, // 9
    0b00000100  // .
};

char digits1[32];
char digits2[32];
uint32_t stream = 0;
float counter=0;
std::string digit;
std::string nextdigit;
long timing = millis();
int brightness = 100;
int char1;
int char2;
int numeral;
int iter = 0;

//uninitalised pointers to SPI objects
SPIClass * hspi = NULL;
static const int spiClk = 1000000; // 1 MHz

void setup() {
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  hspi = new SPIClass(HSPI);
 
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS
  
  //set up slave select pins as outputs as the Arduino API
  //doesn't handle automatically pulling SS low
  pinMode(hspi->pinSS(), OUTPUT); //HSPI SS
  pinMode(OE, OUTPUT);
  Serial.begin(115200);
  radio.begin();
  Serial.println("Booted");
}

void loop() {
  radio.receivePacket();
  disp(analogRead(33)*0.00105006, radio.getRSSI(), iter);
  iter++;
  if(iter>=5)
  {
    iter=0;
  }
}

void disp(float number1, float number2, int addr)
{
  sprintf (digits1, "%.4f", number1);
  sprintf (digits2, "%.4f", number2);
  char1 = 0;
  char2 = 0;
  // Digit 1
  digit = digits1[char1];
  char1++;
  numeral=numbercodes[stoi(digit)];
  nextdigit=digits1[char1];
  if (nextdigit == ".")
  {
    numeral=numeral|numbercodes[10];
    char1++;
  } 
  stream = (numeral<<8) | (0b1<<(3-addr));
  // Digit 2
  digit = digits2[char2];
  char2++;
  numeral=numbercodes[stoi(digit)];
  nextdigit=digits2[char2];
  if (nextdigit == ".")
  {
    numeral=numeral|numbercodes[10];
    char2++;
  } 
  stream = (numeral<<16) | stream;
  timing = micros();
  digitalWrite(OE, HIGH);
  spiCommand(hspi, stream);
  digitalWrite(OE, LOW);
  delayMicroseconds(brightness * (micros() - timing));
}

void spiCommand(SPIClass *spi, uint32_t bits) {
  //use it as you would the regular arduino SPI API
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(spi->pinSS(), HIGH); //pull SS slow to prep other end for transfer
  spi->transfer32(bits);
  digitalWrite(spi->pinSS(), LOW); //pull ss high to signify end of data transfer
  spi->endTransaction();
}