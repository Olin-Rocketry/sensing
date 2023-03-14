class Radio {
  private:
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

    void receivedPacket()
    {
      readRadio();
      decodeData();
      printData();
    }
  
    void readRadio(){
      ;
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
