//Programacion para el sensor uno del primer nivel

// Realiza la lectura en la base de datos del sensor dos del primer nivel para
// de esta manera realizar el promedio de la humedad del sustrato.

// Realiza la activacion de la bomba (Pin 5) y la electrovalvula (Pin 4) para el primer nivel
// Realiza el control de temperatura en el primer nivel minima (Pin 12)
// Realiza el control de temperatura en el primer nivel maxima(Pin 14)

//Código de programación sensor de humedad de suelo capacitivo V1.2
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "FirebaseESP8266.h"

#define AOUT 0 // Pin analógico "A0" para conectar la salida del sensor de humedad capacitivo

// Definimos la direccion host de nuestro servidor
#define FIREBASE_HOST "testesp8266-71a13-default-rtdb.firebaseio.com"
// Definimos el token de nuestro servidor
#define FIREBASE_AUTH "5CYnbceEakCGsRgUj6gUWOz6NX2QMzFWnEF4qmqy"

// Definimos la conexion a punto de acceso wifi
#define WIFI_SSID "Telematics_Tibaduiza"
#define WIFI_PASSWORD "1057597515Tib"

const int Valor_Sensor_Aire = 630; // Valor calculado con el programa de calibración con el sensor al aire
const int Valor_Sensor_Agua = 345; // Valor calculado con el programa de calibración con el sensor sumergido en agua

int valor_sensor = 0; // Variable que almacena el valor de salida del sensor de humedad capacitivo
int valor_sensor2N1 = 0;
int porcentaje = 0; // Variable que almacena el porcentaje de humedad relativa del terreno
int humedadMinN1 = 0; // Variable del sensor de humedad para el nivel 1
int humedadMinN2 = 0; // Variable del sensor de humedad para el nivel 2
int pinBomba = 5;
int pinElectroValvulaNivel1 = 4;
int pinCalefactor = 12;

WiFiClient client;
FirebaseData firebaseData;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println ("");
  Serial.println ("Se conectó al wifi!");
  Serial.println(WiFi.localIP());  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  


}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(0); //connect sensor to Analog 0
  int finporcentaje = map(val, 0, 1023, 100, 0);
  delay(2000);
  Firebase.setString(firebaseData, "humiditySe2N1", String(finporcentaje));
    Serial.println(finporcentaje); 
}
