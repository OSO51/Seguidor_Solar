#define ENCODER_A 23 
#define ENCODER_B 22 

#define LED_Izquierdo 14
#define LED_Derecho 12
#define ZeroError 26

#define PWM1_CH 0
#define PWM2_CH 1
#define PWM_res 8
#define PWM_Freq 1000

volatile int encoder = 0; 
volatile int angulo = 0;

int setpoint = 180;
int error;
int error_speed;
int PWM_Speed;

void IRAM_ATTR encoder_isr() {
  
  int A = digitalRead(ENCODER_A);
  int B = digitalRead(ENCODER_B);

  if ((A == HIGH) != (B == LOW)) {
    encoder--;
  } else {
    encoder++;
  }
  if ((encoder == 1024) || (encoder == -1024)){
    encoder = 0;
  }

  angulo = encoder * (360 / 1024.0);
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

  ledcWrite(PWM1_CH, 0);
  ledcWrite(PWM2_CH, 0);
  delay(10000); 
 
}

void loop() {
  error = setpoint - angulo;

  if(abs(error) >= 60){
    error_speed = 60;
  }else{
    error_speed = abs(error);
  }

  PWM_Speed = error_speed * (255 / 60.0);

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

}












