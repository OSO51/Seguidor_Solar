#define PWMpin 5
#define PWM1_Ch 0
#define PWM_res 8
#define PWM_Freq 1000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

    //Configuración de los canales PWM
  ledcAttachPin(PWMpin, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM_Freq, PWM_res);

  ledcWrite(PWM1_Ch, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWrite(PWM1_Ch, 255);
  delay(3000);
  ledcWrite(PWM1_Ch, 200);
  delay(3000);
  ledcWrite(PWM1_Ch, 150);
  delay(3000);
  ledcWrite(PWM1_Ch, 100);
  delay(3000);
  ledcWrite(PWM1_Ch, 50);
  delay(3000);
  ledcWrite(PWM1_Ch, 0);
  delay(3000);
}
