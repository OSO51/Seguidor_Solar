byte led1 = 2;
byte led2 = 3;
byte led3 = 4;
byte led4 = 5;
byte led5 = 6;
byte led6 = 7;
byte led7 = 8;
byte led8 = 9;
byte Pizq = 10;
byte Pder = 11;

int i;
long vel = 1000;
byte valor = 1;

void setup() {

  for(i=led1;i<=led8;i++){
    pinMode(i,OUTPUT);
  }

  pinMode(Pizq,INPUT);
  pinMode(Pder,INPUT);
}

void loop() {

  if(valor == 0){
    for(i=led1;i<=led8+3;i++)
      if(i<=led8){
        digitalWrite(i,HIGH);
      }
      delay(vel);

      if(i>3){
        digitalWrite(i-3,LOW);
      }
      delay(vel);

      if(digitalRead(Pizq)){
        valor=1;i=led8+3;
      }
      if(digitalRead(Pizq)){
        valor=0;i=led8+3;
      }
  }

  if(valor == 1){
    for(i=led8;i>=led1-3;i--)
      if(i>=led1){
        digitalWrite(i,HIGH);
      }

      if(i<led6){
        digitalWrite(i+3,LOW);
      }
      delay(vel);

      if(digitalRead(Pizq)){
        valor=1;i=led1-3;
      }
      if(digitalRead(Pizq)){
        valor=0;i=led1-3;
      }
  }

}