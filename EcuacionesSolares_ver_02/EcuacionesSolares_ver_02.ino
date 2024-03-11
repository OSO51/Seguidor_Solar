
//RTC + Ecuaciones Solares

#include <RtcDS1302.h>

ThreeWire myWire(25,33,26); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

#define uS_TO_min_FACTOR 60000000  /* Conversion factor for micro seconds to minutes */

//Valores de longitud y latitud en grados decimales
double Long = 99 + (11/60.0) + (8.87/3600.0);
double Lat = 19 + (19/60.0) + (19.64/3600.0);

double AST;
double ET;
double LSTM;

double declination_angle; //delta
double altitude_angle; //Beta_1
double azimuth;  //alpha_1

double amanecer;
double anochecer;
int diaAnno;

//Encoder

#define ENCODER_A 23 // Pin for Encoder A
#define ENCODER_B 22 // Pin for Encoder B

volatile int encoder_value = 0; // Global variable for storing the encoder position
volatile int angle = 0; 

void IRAM_ATTR encoder_isr() {
  
  int A = digitalRead(ENCODER_A);
  int B = digitalRead(ENCODER_B);

  if ((A == HIGH) != (B == LOW)) {
    encoder_value--;
  } else {
    encoder_value++;
  }
  if ((encoder_value == 1024) || (encoder_value == -1024)){
    encoder_value = 0;
  }

  angle = encoder_value * (360 / 1024.0);
}

//PWM Motor
#define LED_IZQ 27
#define LED_DER 14

#define PWM1_Ch 0
#define PWM2_Ch 1
#define PWM_res 8
#define PWM_Freq 31000

int PWM1_DutyCycle = 0;
int PWM2_DutyCycle = 0;


void setup() {
	//RTC
  Serial.begin(115200);
  Rtc.Begin();

  //Encoder
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  attachInterrupt(ENCODER_A, encoder_isr, RISING);

  //Configuración de los canales PWM
  ledcAttachPin(LED_IZQ, PWM1_Ch);
  ledcAttachPin(LED_DER, PWM2_Ch);
  ledcSetup(PWM1_Ch, PWM_Freq, PWM_res);
  ledcSetup(PWM2_Ch, PWM_Freq, PWM_res);
}

void loop() {
  //Fecha del RTC
  RtcDateTime now = Rtc.GetDateTime();
  byte dia = now.Day();
  byte mes = now.Month();
  byte anno = now.Year();

  //Hora local en minutos desde la media noche
  double LST = (now.Hour()*60) + now.Minute() + (now.Second()/60.0);

  //Dia del año
  diaAnno = numeroDia(dia, mes, anno);

  ET = ecuacionTiempo(diaAnno);
  LSTM = 15*int(Long/15.0);
  AST = (LST + (4)*(LSTM - Long) + ET);

  declination_angle = calcDelta(diaAnno);
  altitude_angle = calcAltitud(AST, Lat, declination_angle);
  azimuth = calcAzimuth(AST, altitude_angle, Lat, declination_angle);

  amanecer = sunRise(LSTM, ET, Long, Lat, declination_angle);
  anochecer = sunSet(LSTM, ET, Long, Lat, declination_angle);
  
  //Código para debugging
  /*
  Serial.print("Hora actual : ");
  mostrarHora(LST);
  Serial.print("Angulo de altitud (BETA) : ");
  Serial.println(altitude_angle);
  Serial.print("Azimuth (ALFA) : ");
  Serial.println(azimuth);

  Serial.print("Salida del Sol : ");
  mostrarHora(amanecer);
  Serial.print("Puesta del Sol : ");
  mostrarHora(anochecer);
  */

  //Aqui va el codigo del PID
  

  delay(1000);

  //dormir(anochecer, amanecer);
}


int signo(double valor){
  if(valor > 0 || valor == 0){
    return 1;
  }
  else{
    return -1;
  }
}

void mostrarHora(double minTotal){
  int hora= minTotal/60.0;
  int minutos = minTotal - (hora*60);
  int segundos = (minTotal - (hora*60) - minutos) * 60;
  if (hora<10){
    Serial.print("0");
  }
  Serial.print(hora);

  if (minutos<10){
    Serial.print(":0");
  }else{
    Serial.print(":");
  }
  Serial.print(minutos);

  if (segundos<10){
    Serial.print(":0");
  }else{
    Serial.print(":");
  }
  Serial.println(segundos);
}

double sunRise(double LSTM, double ET, double Longitud, double latitud, double delta){
  double H_s;
  double Sunrise;
  double local_time_Sunrise;
  H_s = (-180.0/PI)*acos(-1*tan((PI/180.0)*latitud)*tan((PI/180.0)*delta));
  Sunrise = 720 + H_s * 4;
  local_time_Sunrise = Sunrise - 4 * (LSTM - Longitud) - ET;
  return local_time_Sunrise;
}

double sunSet(double LSTM, double ET, double Longitud, double latitud, double delta){
  double H_s;
  double Sunset;
  double local_time_Sunset;
  H_s = (180.0/PI)*acos(-1*tan((PI/180.0)*latitud)*tan((PI/180.0)*delta));
  Sunset = 720 + H_s * 4;
  local_time_Sunset = Sunset - 4 * (LSTM - Longitud) - ET;
  return local_time_Sunset;
}

double ecuacionTiempo(int diaAnno){
  double ET;
  double D;
  
  D = 360 * ((diaAnno-81)/365.0);  
  ET = 9.87*sin(2*D*(PI/180.0)) - 7.53*cos(D*(PI/180.0)) - 1.5*sin(D*(PI/180.0));
  return ET;
}

double calcDelta(int diaAnno){
  double delta;
  delta = 23.45 * sin(2*PI* ((diaAnno + 284) / 365.0));
  return delta;
}

double calcAltitud(double AST, double lat, double delta){
  double H;
  double beta;
  H = (AST - 720) / 4.0;
  beta = (180/PI)*asin(cos(lat*(PI/180.0))*cos(delta*(PI/180.0))*cos(H*(PI/180.0))+sin(lat*(PI/180.0))*sin(delta*(PI/180.0)));
  return beta;
}

double calcAzimuth(double AST, double beta,double lat, double delta){
  double H;
  double alfa;
  H = (AST - 720) / 4.0;
  alfa = 180 + signo(H)*(180/PI)*acos((sin(beta*(PI/180.0))*sin(lat*(PI/180.0))-sin(delta*(PI/180.0)))/(cos(beta*(PI/180.0))*cos(lat*(PI/180.0))));
  return alfa;
}

void dormir(double horaDormir, double horaDespertar, int horaActual, int minutoActual){
  int hora = horaDormir/60.0; 
  int minuto = horaDormir - (hora * 60);
  int temporizador = 1440 - horaDormir + horaDespertar;
  if(horaActual == hora && minutoActual == minuto){
    esp_sleep_enable_timer_wakeup(temporizador * uS_TO_min_FACTOR);
    esp_light_sleep_start();
  }
}


int numeroDia(byte day, byte month, byte year){

  int numDia = 0;

  if (month > 1){
    numDia = numDia + 31;
  }
  if (month > 2){
    if (year%4 == 0){
      numDia = numDia + 29;
    }else{
      numDia = numDia + 28;
    }
  }
  if (month > 3){
    numDia = numDia + 31;
  }
  if (month > 4){
    numDia = numDia + 30;
  }
  if (month > 5){
    numDia = numDia + 31;
  }
  if (month > 6){
    numDia = numDia + 30;
  }
  if (month > 7){
    numDia = numDia + 31;
  }
  if (month > 8){
    numDia = numDia + 31;
  }
  if (month > 9){
    numDia = numDia + 30;
  }
  if (month > 10){
    numDia = numDia + 31;
  }
  if (month > 11){
    numDia = numDia + 30;
  }

  numDia = numDia + day;
  return numDia;

}


