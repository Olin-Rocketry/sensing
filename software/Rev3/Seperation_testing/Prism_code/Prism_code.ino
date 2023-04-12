#define Main 1 //MUST change
#define Drog 2 //MUST change
void setup() {
  // put your setup code here, to run once:
  pinMode(Drog,OUTPUT);
  pinMode(Main,OUTPUT);
  digitalWrite(Drog, LOW);
  digitalWrite(Main, LOW);
Serial.begin(9600);
Serial1.begin(9600); //MUST CHANGE
}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial1.available()>0){
  if (Serial1.read()=='d'){
    digitalWrite(Drog, HIGH);
    delay(1000);
    digitalWrite(Drog, LOW);
    Serial.println("BOOM DROG");
  }
  if (Serial1.read()=='m'){
    digitalWrite(Main, HIGH);
    delay(1000);
    digitalWrite(Main, LOW);
    Serial.println("BOOM DROG");
  }
}
}
