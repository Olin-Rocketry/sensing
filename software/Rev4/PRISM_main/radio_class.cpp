#include "radio_class.h"
#include <algorithm>

void Radio::init(){
    //init both un-encoded and encoded packet with 0
    std::fill_n(packet, packetSize, 0.0000);
    std::fill_n(encodedPacket, packetSize * 4, '0');
}

void Radio::led_test(Led *statusLed){
  this->statusLed = statusLed;
  statusLed->RGB2(0, 100, 0, 0);
  statusLed->RGB2(1, 100, 100, 0);
  
}

void Radio::begin(){

    //radio stuff
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    Serial.println("Arduino LoRa RX Test!");
    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    while (!rf95.init())
    {
        Serial.println("LoRa radio init failed");
        statusLed->RGB2(0, 255, 0, 0);
        while (1)
            ;
    }
    Serial.println("LoRa radio init OK!");
    statusLed->RGB2(0, 0, 0, 255);

    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!rf95.setFrequency(RF95_FREQ))
    {
        Serial.println("setFrequency failed");
        while (1)
            ;
    }
    Serial.print("Set Freq to: ");
    Serial.println(RF95_FREQ);

    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    // attempt to speed up radio with Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
    rf95.setModemConfig(1);
    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    rf95.setTxPower(23, false);



     delay(10);
    //initalize EAST serial communication
    // Serial8.begin(800000);  //east uses 8, prism uses 5
    // EAST_serial.begin(Serial8);
}

union Radio::floatunion_t
{
    float f;
    char a[sizeof(float)];
} float_ur;

float Radio::decoder(char *encoded)
{
    // Serial.println("Debug Decoder");
    for (int i = 0; i < sizeof(float); i++)
    {
        float_ur.a[i] = encoded[i];
        // Serial.println(encoded[i]);
    }
    return float_ur.f;
}

void Radio::sendingPacket()
{
    readSerial();

}

 char Radio::readSerial()
 {
//     // if (EAST_serial.available()){
//     //   if(!serial_status){
//     //     statusLed->RGB2(0, 0, 255, 0);
//     //     serial_status = true;
//     //   }
        
// // //        Serial.println("Serial Data:");  
// //         EAST_serial.rxObj(serialBuffer);
// //         Serial.println(sizeof(serialBuffer));
// //         sendRadio(serialBuffer);

        
// //        if (sizeof(serialBuffer) != packetSize * 4)
// //        {
// //            Serial.println("Serial read length mismatch");
// //        }
//     }
// //    else if(!EAST_serial.available())
// //    {
// //      Serial.println("not available");
// //    }
// //    return serialBuffer;
    

 }

void Radio::reveicePacket()
{
    if (rf95.available())
    {
        readRadio();
        decodeData();
        printData();
        
    }
}

void Radio::sendRadio(){
  
//    Serial.println(serialBuffer);
    // Send a message to rf95_server
  if(data->validpacket)
  {
    rf95.send((uint8_t *)data->encodedpacket, packetSize*4);
    data->validpacket=false;
  }
//    delay(10);
//    rf95.waitPacketSent();
//    Serial.println("Packet Sent");
}

void Radio::readRadio()
{
    // Should be a message for us now
    uint8_t serialBuffer[200]; // RH_RF95_MAX_MESSAGE_LEN
    uint8_t len = sizeof(serialBuffer);

    if (rf95.recv(serialBuffer, &len))
    {
        // RH_RF95::printserialBufferfer("Received: ", serialBuffer, len);
        Serial.print("Got: ");
        Serial.println((char *)serialBuffer);
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);
    }
    else
    {
        Serial.println("Receive failed");
    }
    for (int i = 0; i < packetSize * 4; i++)
    {
        //      Serial.println((char)serialBuffer[i]);
        encodedPacket[i] = (char)serialBuffer[i];
    }
}

void Radio::decodeData()
{
    if (sizeof(encodedPacket) / sizeof(encodedPacket[0]) == packetSize * sizeof(float))
    {
        for (int i = 0; i < packetSize; i++)
        {
            char subencodedPacket[sizeof(float)];
            for (int j = 0; j < sizeof(float); j++)
            {
                subencodedPacket[j] = encodedPacket[i * sizeof(float) + j];
            }
            packet[i] = decoder(subencodedPacket);
        }
    }
}

void Radio::printData()
{
    for (int k = 0; k < packetSize; k++)
    {
        Serial.print(packet[k]);
        Serial.print(",");
    }
    Serial.println();
};
