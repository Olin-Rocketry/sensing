#include <algorithm>
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 915.0

class Radio {
  private:    
    RH_RF95 rf95;
    const static int dataPointCount = 27;
    float flightData[dataPointCount];
    char packet[dataPointCount*4];
    
    union floatunion_t {
        float f;
        char a[sizeof (float) ];
    } float_u;

    float decoder(char* encoded)
    {
      //Serial.println("Debug Decoder");
      for (int i = 0; i<sizeof (float); i++)
      {
        float_u.a[i]=encoded[i];
        //Serial.println(encoded[i]);
      }
      return float_u.f;
    }
  public:

    Radio(){
      std::fill_n(flightData, dataPointCount, 0.0);
      RH_RF95 rf95(RFM95_CS, RFM95_INT);
      std::fill_n(packet, dataPointCount*4, '0');
      pinMode(RFM95_RST, OUTPUT);
      digitalWrite(RFM95_RST, HIGH);
//      while (!Serial);
//      Serial.begin(9600);
//      Serial.println("Arduino LoRa RX Test!");
      // manual reset
      digitalWrite(RFM95_RST, LOW);
      delay(10);
      digitalWrite(RFM95_RST, HIGH);
      delay(10);
    
      while (!rf95.init()) {
//        Serial.println("LoRa radio init failed");
        while (1);
      }
//      Serial.println("LoRa radio init OK!");
    
      // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
      if (!rf95.setFrequency(RF95_FREQ)) {
//        Serial.println("setFrequency failed");
        while (1);
      }
//      Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
    
      // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    
      // The default transmitter power is 13dBm, using PA_BOOST.
      // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
      // you can set transmitter powers from 5 to 23 dBm:
      rf95.setTxPower(23, false);
    }

    void sendingPacket()
    {
      sendRadio(readSerial());
    }

    char* readSerial()
    {
      //CHARLIE
      return;
    }

    void sendRadio(char* buffer2){
      // Send a message to rf95_server
      rf95.send((uint8_t *)buffer2, 70);
      delay(10);
      rf95.waitPacketSent();
    }
    
    void receivedPacket()
    {
      readRadio();
      decodeData();
      printData();
    }
  
    void readRadio(){
      if (rf95.available())
      {
        // Should be a message for us now   
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        
        if (rf95.recv(buf, &len))
        {
          //RH_RF95::printBuffer("Received: ", buf, len);
          Serial.print("Got: ");
          Serial.println((char*)buf);
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);
        }
        else
        {
          Serial.println("Receive failed");
        }
      }
    }
    
    void decodeData(){
      if(sizeof(packet)/sizeof(packet[0])==dataPointCount*sizeof(float))
      {
        for (int i=0; i<dataPointCount; i++)
        {
          char subpacket[sizeof(float)];
          for(int j=0; j<sizeof(float); j++)
          {
            subpacket[j]=packet[i*sizeof(float)+j];
          }
          flightData[i]=decoder(subpacket);
        }
      }
    }
    
    void printData(){
      for (int k=0; k<dataPointCount; k++)
      {
        Serial.print(flightData[k]);
        Serial.print(",");
      }
      Serial.println();
    }
};
