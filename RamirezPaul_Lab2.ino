/*
   Laboratorio II
   Autor: Ramirez Paul
   Fecha: 03/12/2020
*/
#include<MsTimer2.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
//Juego de leds
const int leds[4] = {7, 6, 5, 4};
//variables interrupciones
int on = 0; //int 0
int cont;//int 1
int i; //aux
int j = 0; //aux

//Variables reloj
int segundos = 0;
int minutos = 0;
int horas = 0;

//variable potenciometro
const int potPin1 = A0;
const int potPin2 = A1;
int potLectura = 0;
int potLectura2 = 0;
int salida = 0;
int salida2 = 0;

//variables sensores
const int sensor1 = A2;
const int sensor2 = A3;
const int sensor3 = A4;
const int sensor4 = A5;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  attachInterrupt(0, activacion, LOW);
  attachInterrupt(1, contador, LOW);
  //sensores
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  //Leds
  for (i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
  }
  i = 0;


}

void loop() {
  if (on == 2) { //inicio de los modos int1
    //primer modo de encendido de leds
    if (cont == 1) { //inicio modo 1
      while (i < 20) { //mientras transcurran 20 minutos
        i++;
        for (j = 0; j < 4; j++) { //recorrido de leds, encendido y apagado en serie
          digitalWrite(leds[j], HIGH);
          delay(125);
          digitalWrite(leds[j], LOW);
          delay(125);
        }
      }
    }//fin del modo 1

    //segundo modo, cambio de hora
    if (cont == 2) { //inicio modo 2
      //configuracion minutos
      potLectura = analogRead(potPin1);
      salida = map(potLectura, 0, 1023, 0, 59);
      minutos = salida;
      if (minutos < 59)
        minutos++;
      else
        minutos = 0;


      //cofiguracion horas
      potLectura2 = analogRead(potPin2);
      salida2 = map(potLectura2, 0, 1023, 0, 23);
      horas = salida2;
      if (horas < 23)
        horas++;
      else
        horas = 0;
      Serial.println(String(horas) + String(":") + String(minutos + 1));
      delay(1000);
    }//fin modo 2


    //modo de funcionamiento 3
    if (cont == 3) { //inicio modo 3
      if (digitalRead(sensor1) == HIGH || digitalRead(sensor2) == HIGH || digitalRead(sensor3) == HIGH || digitalRead(sensor4) == HIGH) {
        Serial.println("Alarma Activada");
        delay(2000);

      }
    }//fin modo 3
  }
}

//Funcionamiento Reloj
void reloj() {

  if (minutos < 59) {
    minutos++;
  } else {
    minutos = 0;
    if (horas < 23) {
      horas++;
    } else {
      horas = 0;
    }
  }

  //lcd.clear();
  lcd.setCursor(0, 0);
  if (horas < 10) {
    lcd.print("0");
  }
  lcd.print(horas);
  lcd.print(":");
  lcd.setCursor(3, 0);
  if (minutos < 10) {
    lcd.print("0");
  }
  lcd.print(minutos);
  //  lcd.print(":");
  lcd.setCursor(6, 0);
  //  if(segundos<10){
  //    lcd.print("0");
  //  }
  //  lcd.print(segundos);


  //lcd.print(String(horas)+String(":")+String(minutos)+String(":")+String(segundos));

}

//metodo int0
void activacion() {
  switch (on) {
    case 0:
      MsTimer2::set(1000, reloj);
      MsTimer2::start();
      Serial.println("Encendido: Inicio de Sistema");
      Serial.println("Elija el modo de funcionamiento: ");
      on++;
      break;

    case 1:
      Serial.println("Iniciar modo seleccionado");
      on++;
      break;

    case 2:
      Serial.println("Apagar: Sistema apagado");
      cont = 0;
      i = 0;
      j = 0;
      on = 0;
      break;
  }
}



//metodo int1
void contador() {
  if (on == 1) { //validar int0
    cont++;
    if (cont == 1) {
      Serial.println(String("Modo ") + String(cont) + String(": Encernder Leds"));
    }
    if (cont == 2) {
      Serial.println(String("Modo ") + String(cont) + String(": Ajustar Hora"));
    }
    if (cont == 3) {
      Serial.println(String("Modo ") + String(cont) + String(": Sensores"));
      //cont=0;
    }
    if (cont == 4) {
      Serial.println("Elija el modo de funcionamiento: ");
      cont = 0;
    }



  }
}
