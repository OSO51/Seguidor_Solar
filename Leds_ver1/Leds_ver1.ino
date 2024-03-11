byte led1 = 2;
byte led2 = 3;
byte led3 = 4;
byte led4 = 5;
byte led5 = 6;
byte led6 = 7;
byte led7 = 8;
byte led8 = 9;

int i;
long vel=100;

void setup() {

  for(i=led1;i<=led8;i++){
    pinMode(i,OUTPUT);
  }

}

void loop() {

  for(i=led1;i<=8;i++){
    digitalWrite(i,HIGH);
    delay(vel);
  }

  for(i=led8;i>=led1;i--){
    digitalWrite(i,LOW);
    delay(vel);
  }

  digitalWrite(led4,1);
  digitalWrite(led5,1);
  delay(1000);

  for(i=6;i<=8;i++){
    digitalWrite(i,LOW);
    digitalWrite(i+1,HIGH);
    digitalWrite(11-i,LOW);
    digitalWrite(10-i,HIGH);
    delay(vel*2);
  }

}
