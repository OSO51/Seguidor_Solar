#include <ESP32Time.h>

//ESP32Time rtc;
ESP32Time rtc(0);

#define uS_TO_min_FACTOR 60000000  /* Conversion factor for micro seconds to minutes */

void setup() {
	// Start the serial interface
	Serial.begin(115200);
  delay(1000);
  rtc.setTime(0, 18, 13, 29, 6, 2023);

  Serial.println("Esto se imprime en el setup.");

}

void loop() {

  double Long = 99 + (11/60.0) + (8.24/3600.0);
  double Lat = 19 + (19/60.0) + (21.06/3600.0);
  double LST = (rtc.getHour(true)*60) + rtc.getMinute() + (rtc.getSecond()/60.0);
  double AST;
  double ET;
  double LSTM;

  double declination_angle; //delta
  double altitude_angle; //Beta_1
  double azimuth;  //alpha_1

  double amanecer;
  double anochecer;
  
  ET = ecuacionTiempo();
  LSTM = 15*int(Long/15.0);
  AST = (LST + (4)*(LSTM - Long) + ET);

  declination_angle = calcDelta();
  altitude_angle = calcAltitud(AST, Lat, declination_angle);
  azimuth = calcAzimuth(AST, altitude_angle, Lat, declination_angle);

  amanecer = sunRise(LSTM, ET, Long, Lat, declination_angle);
  anochecer = sunSet(LSTM, ET, Long, Lat, declination_angle);
  
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

  delay(1000);

  //dormir(anochecer, amanecer);

  Serial.println("");
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

double ecuacionTiempo(){
  double ET;
  double D;
  int N;
  N = rtc.getDayofYear();
  D = 360 * ((N-81)/365.0);  
  ET = 9.87*sin(2*D*(PI/180.0)) - 7.53*cos(D*(PI/180.0)) - 1.5*sin(D*(PI/180.0));
  return ET;
}

double calcDelta(){
  double delta;
  delta = 23.45 * sin(2*PI*((rtc.getDayofYear()+284)/365.0));
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

void dormir(double horaDormir, double horaDespertar){
  int hora = horaDormir/60.0; 
  int minuto = horaDormir - (hora * 60);
  int temporizador = 1440 - horaDormir + horaDespertar;
  if(rtc.getHour(true) == hora && rtc.getMinute() == minuto){
    esp_sleep_enable_timer_wakeup(temporizador * uS_TO_min_FACTOR);
    esp_light_sleep_start();
  }
}
