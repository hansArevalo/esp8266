//Programacion para el sensor uno del primer nivel

// Realiza la lectura en la base de datos del sensor dos del primer nivel para
// de esta manera realizar el promedio de la humedad del sustrato.

// Realiza la activacion de la bomba (Pin 5) y la electrovalvula (Pin 4) para el primer nivel
// Realiza el control de temperatura de los dos niveles - minima (Pin 12)
// Realiza el control de temperatura de los dos niveles - maxima(Pin 14)
// Realiza la activacion de la bomba si la humedad relativa es muy baja en el nivel 1 - el tiempo de apertura de la valvula es el mismo que la del sustrato 

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
// Sensores de temperatura y humedad
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

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
int pinVentilacion = 14;
int temperaturaMin = 0;
int temperaturaMax = 0;
int temperaturaMinN2 = 0;
int temperaturaMaxN2 = 0;
int promedioHumedad = 0;
int humedadRelativaMin = 0;

WiFiClient client;
FirebaseData firebaseData;


void setup() {
  Serial.begin(9600); // open serial port, set the baud rate as 9600 bps

  Serial.println(F("Test calibracion DHT"));
  dht.begin();

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("");
  Serial.println ("Se conectó al wifi!");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Configuracion de los pines
  pinMode(pinBomba, OUTPUT); // pin 16 activacion de la bomba
  pinMode(pinElectroValvulaNivel1, OUTPUT);
  pinMode(pinCalefactor, OUTPUT);
  pinMode(pinVentilacion, OUTPUT);

}
void loop() {
  //-----------inicio
  //  // lectura de la base de datos

  Firebase.getInt(firebaseData, "/humiditySe2N1");
  valor_sensor2N1 = firebaseData.intData();
  delay(250);
  Firebase.getInt(firebaseData, "/humidityMin");
  humedadMinN1 = firebaseData.intData();
  delay(250);
  Firebase.getInt(firebaseData, "/humidityMinN2");
  humedadMinN2 = firebaseData.intData();
  delay(250);
  Firebase.getInt(firebaseData, "/temperatureMin");
  temperaturaMin = firebaseData.intData();
  delay(250);
  Firebase.getInt(firebaseData, "/temperatureMax");
  temperaturaMax = firebaseData.intData();
  delay(250);
  Firebase.getInt(firebaseData, "/humidityReMinN1");
  humedadRelativaMin = firebaseData.intData();
  delay(250);
   Firebase.getInt(firebaseData, "/temperatureMinN2");
  temperaturaMinN2 = firebaseData.intData();
  delay(250);
  Firebase.getInt(firebaseData, "/temperatureMaxN2");
  temperaturaMaxN2 = firebaseData.intData();
  delay(250);

  int val = analogRead(0); //connect sensor to Analog 0
  int finporcentaje = map(val, 0, 1023, 100, 0);
  delay(2000);

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int f = dht.readTemperature(true);
  Serial.print("Valor del sensor capacitivo: ");
  Serial.print(val);
  Serial.println(" ");
  Serial.print("Porcentaje del sensor capacitivo: ");
  Serial.print(finporcentaje);
  Serial.println("%");
  Serial.println(valor_sensor2N1);
  // condicionales para activacion de la bomba

  promedioHumedad = (finporcentaje + valor_sensor2N1);
  promedioHumedad = promedioHumedad / 2;
  Firebase.setString(firebaseData, "humidity", String(promedioHumedad));
  Firebase.setString(firebaseData, "temperature", String(t));
  Firebase.setString(firebaseData, "humidityRe", String(h));
  Serial.println (finporcentaje);
  if (promedioHumedad <= humedadMinN1 or h<=humedadRelativaMin) {
    activarBomba();
  }
  if (t <= temperaturaMin or t <= temperaturaMinN2 ) {
    activarCalefaccion();
  }
  if (t >= temperaturaMax or t >= temperaturaMaxN2) {
    activarVentilacion();
  }
}

// metodo para activar la bomba por 1500 milisegundos
void activarBomba() {
  digitalWrite(pinBomba, HIGH);// se activa la bomba
  digitalWrite(pinElectroValvulaNivel1, HIGH);
  delay(1500); // tiempo de encencido de la bomba y electro valvula
  digitalWrite(pinBomba, LOW);
  digitalWrite(pinElectroValvulaNivel1, LOW);
  delay(7000);
}

void activarCalefaccion() {
  digitalWrite(pinCalefactor, HIGH);// se activa la bomba
  delay(1500); // tiempo de encencido de la bomba y electro valvula
  digitalWrite(pinCalefactor, LOW);
}

void activarVentilacion() {
  Serial.println (pinVentilacion);
  digitalWrite(pinVentilacion, HIGH);// se activa la bomba
  delay(1500); // tiempo de encencido de la bomba y electro valvula
  digitalWrite(pinVentilacion, LOW);
}
