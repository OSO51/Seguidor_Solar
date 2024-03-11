void setup() {
  float D;
  int N;
  double ET;
  float LSTM;
  float Long = 99 + (7/60.0) + (59.15/3600.0);
  float Lat = 19 + (25/60.0) + (54.84/3600.0);
  float LST = (15*60)+45+(0/60.0);
  float AST;

  float H;
  float declination_angle; //delta
  float altitude_angle; //Beta_1
  float azimuth;  //alpha_1

  Serial.begin(115200);

  //Ejemplo 1
  N = 178;
  D = 360.0*((N-81.0)/365.0);  
  ET = 9.87*sin(2*D*(PI/180.0))-7.53*cos(D*(PI/180.0))-1.5*sin(D*(PI/180.0));
  //LSTM = 15*int((Long/15.0)+0.5);
  LSTM = 15*int(Long/15.0);
  AST = (LST + (4)*(LSTM - Long) + ET); //Da el resultado en minutos. El 0.5 es para que lleve a cabo el redondeo.

  Serial.println("Ejercicio 1:");
  Serial.print("D ->");
  Serial.println(D);
  Serial.print("ET ->");
  Serial.println(ET);
  Serial.print("LSTM ->");
  Serial.println(LSTM);
  Serial.println("Hora Solar Aparente");
  mostrarHora(AST);

  

  //Ejemplo 2
  H = (AST-720)/4.0;
  declination_angle = 23.45 * sin(2*PI*((N+284)/365.0));
  altitude_angle = (180/PI)*asin(cos(Lat*(PI/180.0))*cos(declination_angle*(PI/180.0))*cos(H*(PI/180.0))+sin(Lat*(PI/180.0))*sin(declination_angle*(PI/180.0)));
  azimuth = 180 + (signo(H)*(180/PI)*acos((sin(altitude_angle*(PI/180.0))*sin(Lat*(PI/180.0))-sin(declination_angle*(PI/180.0)))/(cos(altitude_angle*(PI/180.0))*cos(Lat*(PI/180.0)))));

  Serial.println("");
  Serial.println("Ejercicio 2:");
  Serial.print("H ->");
  Serial.println(H);
  Serial.print("Angulo de declinacion DELTA ->");
  Serial.println(declination_angle);
  Serial.print("Angulo de altitud BETA->");
  Serial.println(altitude_angle);
  Serial.print("Azimuth ALFA ->");
  Serial.println(azimuth);

}

void loop() {

}

int signo(float valor){
  if(valor > 0 || valor == 0){
    return 1;
  }
  if(valor < 0){
    return -1;
  }
  Serial.println("Error signo del angulo = 0");
}

void mostrarHora(float minTotal){
  int hora= minTotal/60.0;
  int minutos = minTotal - (hora*60);
  int segundos = (minTotal - (hora*60) - minutos) * 60;

  if (hora<10){
    Serial.print("Hora: 0");
  }else{
    Serial.print("Hora: ");
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


