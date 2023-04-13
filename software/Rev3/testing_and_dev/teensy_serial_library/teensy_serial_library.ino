#include <SerialTransfer.h>



SerialTransfer myTransfer_send;
SerialTransfer myTransfer_recive;

struct __attribute__((packed)) STRUCT1 {
  char z;
} testStruct_send;

struct __attribute__((packed)) STRUCT2 {
  char z;
} testStruct_recive;

char message[2];

void setup()
{
  Serial.begin(115200);
  Serial5.begin(115200);
  Serial8.begin(115200);
  myTransfer_send.begin(Serial5);
  myTransfer_recive.begin(Serial8);

  testStruct_send.z = 'hi';

}

void loop(){

  
  myTransfer_send.sendDatum(testStruct_send);
  delay(50);
  if(myTransfer_recive.available()){
    myTransfer_recive.rxObj(testStruct_recive);
    Serial.println(testStruct_recive.z);
    //Serial.println(testStruct_recive.y);m
}


}
