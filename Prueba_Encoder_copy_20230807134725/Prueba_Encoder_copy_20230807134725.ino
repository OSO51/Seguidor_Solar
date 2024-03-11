#define ENCODER_A 12 
#define ENCODER_B 13 

#define LED_Izquierdo 14
#define LED_Derecho 12
#define ZeroError 2-6

#define PWM1_CH 0
#define PWM2_CH 1
#define PWM_res 8
#define PWM_Freq 30000

volatile int encoder = 0; 
volatile int angulo = 0;

int setpoint = 180;
int error;
int PWM_Speed;

void IRAM_ATTR encoder_isr() {
  
  int A = digitalRead(ENCODER_A);
  int B = digitalRead(ENCODER_B);

  if ((A == HIGH) != (B == LOW)) {
    encoder--;
  } else {
    encoder++;
  }
  if ((encoder == 100) || (encoder == -100)){
    encoder = 0;
  }

  angulo = encoder * (360 / 100.0);
}


void setup() {
  Serial.begin(115200); 
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ZeroError, OUTPUT);
  attachInterrupt(ENCODER_A, encoder_isr, RISING);

  ledcAttachPin(LED_Izquierdo, PWM1_CH);
  ledcAttachPin(LED_Derecho, PWM2_CH);
  ledcSetup(PWM1_CH, PWM_Freq, PWM_res);
  ledcSetup(PWM2_CH, PWM_Freq, PWM_res); 
 
}

void loop() {
  error = setpoint - angulo;

  PWM_Speed = 100;

  if(error > 0){
    ledcWrite(PWM1_CH, PWM_Speed);
    ledcWrite(PWM2_CH, 0);
    digitalWrite(ZeroError, LOW);
  }
  if(error < 0){
    ledcWrite(PWM1_CH, 0);
    ledcWrite(PWM2_CH, PWM_Speed);
    digitalWrite(ZeroError, LOW);
  }
  if(error == 0){
    ledcWrite(PWM1_CH, 0);
    ledcWrite(PWM2_CH, 0);
    digitalWrite(ZeroError, HIGH);
  }

  Serial.println("Angulo: " + String(angulo) + " || Error: "+String(error) + "  || PWM: " + String(PWM_Speed));

  delay(10);
}












