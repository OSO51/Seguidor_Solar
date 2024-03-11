
//configurar el LCD
void(* reset) (void)=0;

int ref = 9; //ajuste de referencia
int rst = 8; //reset
int A = 12; //dt
int B = 13; //clk
int motorA=32;
int motorB=25;
int pul;
int Anterior = 0;
volatile int posicion = 0;
int vuelta = 0;

int aux;

void setup() {
  pinMode(A,INPUT);
  pinMode(B,INPUT);
  pinMode(motorA,OUTPUT);
  pinMode(motorB,OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(A),encoder,LOW);
  Serial.println("Listo");
  pul=Serial.read();
}

void loop() 
{
 if(pul<posicion)
 {
  motorA=1;
  motorB=0;
 }
 if(pul>posicion)
 {
  motorA=0;
  motorB=1;
 }
 if (digitalRead(rst)==0)
 {
  reset();
 }
 if(pul=posicion)
 {
  motorA=0;
  motorB=0;
 }
 Anterior= posicion;
}
void encoder()
{
  static unsigned long ultimaInterrupcion=0;
  unsigned long tiempoInterrupcion=millis();

  if(tiempoInterrupcion-ultimaInterrupcion > 5){
  if(digitalRead(B)==HIGH)
  {
    posicion++;
  }
  else
  {
    posicion--;
  }
  posicion=min (0 , max (17408 , aux = posicion));
  ultimaInterrupcion=tiempoInterrupcion;
  if(posicion==20)
  {
    vuelta++;
    posicion=0;
  }
  if(posicion==-20)
  {
    vuelta--;
    posicion=0;
  }
  }
}