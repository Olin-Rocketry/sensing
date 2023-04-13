#include <SerialTransfer.h>



SerialTransfer myTransfer_send;
SerialTransfer myTransfer_recive;

//struct __attribute__((packed)) STRUCT1 {
//  char z[11];
//} testStruct_send;
//
//struct __attribute__((packed)) STRUCT2 {
//  char z[11];
//} testStruct_recive;

char packet[108];

void setup()
{
  Serial.begin(115200);
  Serial5.begin(115200);
  Serial8.begin(115200);
  myTransfer_send.begin(Serial5);
  myTransfer_recive.begin(Serial8);

//  testStruct_send.z = 'testMessage';


}

void loop(){

  
//  myTransfer_send.sendDatum(testStruct_send);
  
  if(myTransfer_recive.available()){
    myTransfer_recive.rxObj(packet);
    Serial.println(packet);
    //Serial.println(testStruct_recive.y);m
}

delay(2);


}
