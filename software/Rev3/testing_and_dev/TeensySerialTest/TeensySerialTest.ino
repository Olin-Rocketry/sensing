
const int BUFFER_SIZE=1;
char encodedBatch_send[6]="hello";
char encodedBatch_recivce[80];
int rlen;
void setup( )  
{  
Serial2.begin(9600); 
Serial3.begin(9600); 
}  
void loop( ){  
  
  
if(Serial2.available( ) > 0)  
{  
  rlen = Serial2.readBytes(encodedBatch_recivce, 5); 
  // = Serial2.readString();
  //arrivingdatabyte = Serial2.read( );  // It will read the incoming or arriving data byte  
  Serial.print("data byte received:");  
  Serial.println(encodedBatch_recivce);
   
   
}  
Serial3.write(encodedBatch_send);
delay(10);
}  
