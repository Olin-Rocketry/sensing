#define Main 24 //MUST change
#define Drog 25 //MUST change
void setup() {
  // put your setup code here, to run once:
  pinMode(Drog,OUTPUT);
  pinMode(Main,OUTPUT);
  digitalWrite(Drog, LOW);
  digitalWrite(Main, LOW);
Serial.begin(9600);
Serial5.begin(9600); //MUST CHANGE
}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial5.available()>0){
  char character=Serial5.read();
  if (character=='d'){
    digitalWrite(Drog, HIGH);
    delay(1000);
    digitalWrite(Drog, LOW);
    Serial.println("BOOM DROG");
  }
  if (character=='m'){
    digitalWrite(Main, HIGH);
    delay(1000);
    digitalWrite(Main, LOW);
    Serial.println("BOOM Main");
  }
}
}
