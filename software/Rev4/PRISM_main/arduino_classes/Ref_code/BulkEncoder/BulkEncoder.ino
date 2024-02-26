union floatunion_t {
    float f;
    char a[sizeof (float) ];
} float_u;

void bulkencode(char* in, char* out) {
  //in and out should be char arrays of the same size
  for (int i=0; i<sizeof(in)/sizeof(in[0]); i++)
  {
    char encoded[sizeof (float) ];
    encoder(encoded, in[i]);
    encoder(encoded, in[i]);
    for(int j=0; j<sizeof(float); j++)
    {
      out[i*sizeof(float)+j]=encoded[j];
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
