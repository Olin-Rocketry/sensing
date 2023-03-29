void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

union floatunion_t {
    float f;
    char a[sizeof (float) ];
} float_u;

void loop() {
  // put your main code here, to run repeatedly:
  //float pi=rand();
  int quantity=27;
  float pi[quantity];
  char packet[sizeof(float)*quantity];
  for (int i=0; i<quantity; i++)
  {
    pi[i]=rand();
  }

  Serial.println("***");
  for (int i=0; i<sizeof(pi)/sizeof(pi[0]); i++)
  {
    char encoded[sizeof (float) ];
    encoder(encoded, pi[i]);
    encoder(encoded, pi[i]);
    float temp=decoder(encoded);
    
    //Serial.println(String(pi[i]==temp)+"   "+String(pi[i])+" = "+String(temp));

    for(int j=0; j<sizeof(float); j++)
    {
      packet[i*sizeof(float)+j]=encoded[j];
    }
  }
  Serial.println("***");
  
  Serial.println("Encoded");
  Serial.print(String(sizeof(packet)/sizeof(packet[0]))+": ");
  for (int k=0; k<sizeof(packet)/sizeof(packet[0]); k++)
  {
    Serial.print(packet[k]);
  }
  Serial.println();
  
  if(sizeof(packet)/sizeof(packet[0])==quantity*sizeof(float))
  {
    float out[quantity];
    for (int i=0; i<quantity; i++)
    {
      char subpacket[sizeof(float)];
      for(int j=0; j<sizeof(float); j++)
      {
        subpacket[j]=packet[i*sizeof(float)+j];
      }
      out[i]=decoder(subpacket);
    }
  
    int counterrors=0;
    for (int k=0; k<quantity; k++)
    {
      Serial.println(String(pi[k]==out[k])+"   "+String(pi[k])+" = "+String(out[k]));
      counterrors+=(pi[k]==out[k]);
    }
    Serial.println("Percent success: "+String(counterrors/quantity*100)+"%");
  }
  delay(1000);
}

void encoder(char* encoded, float input)
{
  float_u.f=input;
  //Serial.println("Debug Encoder");
  for (int i = 0; i<sizeof (float); i++)
  {
    encoded[i] = float_u.a[i];
    //Serial.println(encoded[i]);
  }
}

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
