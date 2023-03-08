union floatunion_t {
    float f;
    char a[sizeof (float) ];
} float_u;

char * bulkencode(input) {
  for (int i=0; i<sizeof(input)/sizeof(input[0]); i++)
  {
    char encoded[sizeof (float) ];
//    encoder(encoded, input[i]);
//    encoder(encoded, input[i]);
    for(int j=0; j<sizeof(float); j++)
    {
      packet[i*sizeof(float)+j]=encoded[j];
    }
  }
//  Serial.println("Encoded");
//  Serial.print(String(sizeof(packet)/sizeof(packet[0]))+": ");
//  for (int k=0; k<sizeof(packet)/sizeof(packet[0]); k++)
//  {
//    Serial.print(packet[k]);
//  }
}

void encoder(char* encoded, float input)
{
  float_u.f=input;
  for (int i = 0; i<sizeof (float); i++)
  {
    encoded[i] = float_u.a[i];
  }
}
