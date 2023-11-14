#include <Wire.h>
#include <DHT.h>
#include "MQ135.h"
#include <LiquidCrystal.h>


#define DHTPIN 52      // Pin de datos del DHT22
#define DHTTYPE DHT22  // Tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

const int rs = 24, en = 25, d4 = 34, d5 = 32, d6 = 30, d7 = 28;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

MQ135 gasSensor = MQ135(A0);
const int relayPin = 22;               // Pin del relé
const int ventiladorPin = 53;          // Pin del ventilador
const float temperaturaUmbral = 35.0;  // Temperatura máxima permitida

float temperatura = 0.0;
float humedad = 0.0;
float ppm = 0.0;


void setup() {
  lcd.begin(16, 2);
  lcd.print("Control Temp");
  Wire.begin();                // Dirección del esclavo
  pinMode(relayPin, OUTPUT);
  pinMode(ventiladorPin, OUTPUT);
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  delay(2000);  // Espera un momento entre lecturas

  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();
  ppm = gasSensor.getPPM();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print(" %");


  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  Serial.print("ppm CO2: ");
  Serial.println(ppm);

  // Lógica para el relé basada en la temperatura
  if (temperatura > temperaturaUmbral) {
    digitalWrite(relayPin, HIGH);  // Enciende la bombilla
    Serial.println("Encender Bombilla");
  } else {
    digitalWrite(relayPin, LOW);  // Apaga la bombilla
    Serial.println("Apagar Bombilla");
  }

  // Lógica para el ventilador basada en la concentración de CO2
  if (ppm >= 113) {
    digitalWrite(ventiladorPin, HIGH);  // Enciende el ventilador
    Serial.println("¡Alerta! Concentración de CO2 alta. Encendiendo el ventilador.");
  } else {
    digitalWrite(ventiladorPin, LOW);  // Apaga el ventilador
  }
  Wire.beginTransmission(8);  // Dirección del maestro (ESP32)
  Wire.write((uint8_t*)&temperatura, sizeof(temperatura));
  Wire.write((uint8_t*)&humedad, sizeof(humedad));
  Wire.write((uint8_t*)&ppm, sizeof(ppm));
  Wire.endTransmission();
  Serial.println("enviando");
}
void enviarDatos() {
  Wire.beginTransmission(8);  // Dirección del maestro (ESP32)
  Wire.write((uint8_t*)&temperatura, sizeof(temperatura));
  Wire.write((uint8_t*)&humedad, sizeof(humedad));
  Wire.write((uint8_t*)&ppm, sizeof(ppm));
  Wire.endTransmission();
  Serial.println("enviando");
}
