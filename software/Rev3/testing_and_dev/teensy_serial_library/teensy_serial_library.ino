#include "SerialTransfer.h"


SerialTransfer myTransfer_send;
SerialTransfer myTransfer_recive;

struct __attribute__((packed)) STRUCT1 {
  char z;
  float y;
} testStruct_send;

struct __attribute__((packed)) STRUCT2 {
  char z;
  //float y;
} testStruct_recive;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  myTransfer_send.begin(Serial1);
  myTransfer_recive.begin(Serial2);

  testStruct_send.z = '$';
  testStruct_send.y = 4.5;
}

void loop()
{
  myTransfer_send.sendDatum('c');
  delay(50);
  if(myTransfer_recive.available()){
    myTransfer_recive.rxObj(testStruct_recive);
    Serial.print(testStruct_recive.z);
    //Serial.println(testStruct_recive.y);
}
}