//To Fire Main type "Main!"
//To fire Drog type "Drog!"

#include <SPI.h>
#include <RH_RF95.h>
//#define RFM69_RST     3   // "A"
//#define RFM69_CS      10   // "B"
//#define RFM69_IRQ     4    // "C"
//#define RFM69_IRQN    digitalPinToInterrupt(RFM69_IRQ )
//

#define RFM95_CS 10
#define RFM95_RST 2
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
String  firestring_main="Main!";
String  firestring_drogue="Drog!";
int fire_main=0;
int fire_drog=0;
void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  pinMode(A19, OUTPUT);
  pinMode(A20, OUTPUT);
   pinMode(A7, OUTPUT);
   pinMode(7,OUTPUT);
   digitalWrite(7, HIGH);
         analogWrite(A19, LOW);
        analogWrite(A20, LOW);
  //pinMode(13, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  delay(100);

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
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission
void loop()
{
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  
  char radiopacket[20] = "Hello World #      ";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  radiopacket[19] = 0;
  
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)radiopacket, 20);

  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply..."); delay(10);
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      
      if  (firestring_drogue.equals(String((char *)buf))&& fire_drog==0){
        digitalWrite(A19, HIGH);
        delay(1000);
         digitalWrite(A19, LOW);
        //digitalWrite(13, HIGH);
         Serial.println("Boom Drogue");
         char radiopacket[20] = "BOOM Drogue # ";
          itoa(packetnum++, radiopacket+13, 10);
          Serial.print("Sending "); Serial.println(radiopacket);
          radiopacket[19] = 0;
          
          Serial.println("Sending..."); delay(10);
          rf95.send((uint8_t *)radiopacket, 20);
        
          Serial.println("Waiting for packet to complete..."); delay(10);
          rf95.waitPacketSent();
          fire_drog=1;
          // Now wait for a reply
      }
      else if  (firestring_main.equals(String((char *)buf))&& fire_main==0){
        
        digitalWrite(A20, HIGH);
        delay(1000);
        digitalWrite(A20, LOW);
         Serial.println("Boom Main");
         char radiopacket[20] = "BOOM main # ";
          itoa(packetnum++, radiopacket+13, 10);
          Serial.print("Sending "); Serial.println(radiopacket);
          radiopacket[19] = 0;
          
          Serial.println("Sending..."); delay(10);
          rf95.send((uint8_t *)radiopacket, 20);
        
          Serial.println("Waiting for packet to complete..."); delay(10);
          rf95.waitPacketSent();
          // Now wait for a reply
          fire_main=1;
      }
      else{
         digitalWrite(A19, LOW);
        digitalWrite(A20, LOW);
      }
    }
    else
    {
      Serial.println("Receive failed");
    }
  }

  else
  {
    Serial.println("No reply, is there a listener around?");
  }
  delay(1000);
}
