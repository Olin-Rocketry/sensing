#include <iostream>
#include <string>
using namespace std;
#include <SPI.h>

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
  pinMode(OE, OUTPUT);SC
  Serial.begin(115200);
  radio.begin();
  Serial.println("Booted");
}

void loop() {
  radio.receivePacket();
  disp(analogRead(33)*0.00105006, 1000, iter);
  iter++;
  if(iter>=5)
  {
    iter=0;
  }
}

void disp(float number1, float number2, int addr)
{
  
}

void spiCommand(SPIClass *spi, uint32_t bits) {
  //use it as you would the regular arduino SPI API
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(spi->pinSS(), HIGH); //pull SS slow to prep other end for transfer
  spi->transfer32(bits);
  digitalWrite(spi->pinSS(), LOW); //pull ss high to signify end of data transfer
  spi->endTransaction();
}
