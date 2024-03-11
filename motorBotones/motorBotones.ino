#define ENCODER_A 12
#define ENCODER_B 13

#define LED_IZQ 32 //Morado
#define LED_DER 25 //Blanco

#define PWM1_Ch 0
#define PWM2_Ch 1
#define PWM_res 8
#define PWM_Freq 1000

#define BTN_IZQ 19
#define BTN_CTR 5
#define BTN_DER 18

int encoder = 0;
//int angulo = 0;
int PWM_Speed = 0;
int B = 0;

int velocidad = 0;
int error = 0;


/*Interrupción asignada al canal A del encoder con flanco de subida
Cuando se realiza la interrupción la variable B recibe el estado del encoder B
*/
void IRAM_ATTR encoder_isr() {
  B = digitalRead(ENCODER_B);
  if (B == 1) {
    encoder++;
  } 
  if(B == 0){
    encoder--;
  }
  //angulo = encoder * (360 / 28800.0);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  //Configuración de los canales PWM
  ledcSetup(PWM1_Ch, PWM_Freq, PWM_res);
  ledcSetup(PWM2_Ch, PWM_Freq, PWM_res);
  ledcAttachPin(LED_IZQ, PWM1_Ch);
  ledcAttachPin(LED_DER, PWM2_Ch);


  //Encoder
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);

  attachInterrupt(ENCODER_A, encoder_isr, RISING);

  ledcWrite(PWM1_Ch, 0);
  ledcWrite(PWM2_Ch, 0);

}

void loop() {
  Serial.println("empieza loop");
  delay(1000);
  moverMotor(14400);
  delay(1000);
  moverMotor(0);

  Serial.println("Termino");
  delay(100000);
   
}

void moverMotor(int objetivo){
  int sentido = 0;
  int sentido_anterior = 0;
  while(encoder != objetivo){
    if(encoder > objetivo){
      sentido = 1;
      if((sentido + sentido_anterior) == 0){
        ledcWrite(PWM2_Ch, 0);
        delay(5);
      }
      ledcWrite(PWM1_Ch, PWMSpeed(objetivo));
      
    }else{
      sentido = -1;
      if((sentido + sentido_anterior) == 0){
        ledcWrite(PWM1_Ch, 0);
        delay(5);
      }
      ledcWrite(PWM2_Ch, PWMSpeed(objetivo));

    }
    sentido_anterior = sentido;
    ledcWrite(PWM1_Ch, 0);
    ledcWrite(PWM2_Ch, 0);
    delay(5);
  }
}

int PWMSpeed(int objetivo){
  velocidad = 0;
  error = objetivo - encoder;
  velocidad = 255*(abs(error)/17408.0);
  if(velocidad >= 255){
    velocidad = 255;
    return velocidad;
  }
  if(velocidad <=50){
    velocidad = 50;
    return velocidad;
  }
}




