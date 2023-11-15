#include <Wire.h>
#include <DHT.h>
#include "MQ135.h"
#include <LiquidCrystal.h>

// Definiciones para el sensor DHT
#define DHTPIN 52      // Pin de datos del DHT22
#define DHTTYPE DHT22  // Tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Definiciones para la pantalla LCD
const int rs = 24, en = 25, d4 = 34, d5 = 32, d6 = 30, d7 = 28;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Definiciones para el sensor de gas MQ135
MQ135 gasSensor = MQ135(A0);

// Definiciones para el relé y el ventilador
const int relayPin = 22;               // Pin del relé
const int ventiladorPin = 53;          // Pin del ventilador

// Umbral de temperatura para encender la bombilla
const float temperaturaUmbral = 35.0;

// Umbrales para el CO2 y la humedad
const int co2Umbral = 300;   // CO2 máxima permitida
const int humedadUmbral = 65;   // Humedad máxima permitida

// Variables para almacenar datos del entorno
float temperatura = 0.0;
float humedad = 0.0;
float ppm = 0.0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Control Temp");
  Wire.begin();                // Inicia la comunicación I2C
  pinMode(relayPin, OUTPUT);
  pinMode(ventiladorPin, OUTPUT);
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  delay(2000);  // Espera un momento entre lecturas

  // Lee los datos del sensor DHT y MQ135
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();
  ppm = gasSensor.getPPM();

  // Muestra los datos en la pantalla LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print(" %");

  // Muestra los datos en el puerto serie
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

  // Lógica para el ventilador basada en la concentración de CO2 y humedad
  if (ppm >= co2Umbral) {
    digitalWrite(ventiladorPin, HIGH);  // Enciende el ventilador
    Serial.println("¡Alerta! Concentración de CO2 alta. Encendiendo el ventilador.");
  } else if (humedad >= humedadUmbral) {
    digitalWrite(ventiladorPin, HIGH);  // Enciende el ventilador
  } else {
    digitalWrite(ventiladorPin, LOW);  // Apaga el ventilador
  }

  // Envía los datos al otro microcontrolador a través de I2C
  enviarDatos();
}

// Función para enviar datos a través de I2C
void enviarDatos() {
  Wire.beginTransmission(8);  // Dirección del maestro (ESP32)
  Wire.write((uint8_t*)&temperatura, sizeof(temperatura));
  Wire.write((uint8_t*)&humedad, sizeof(humedad));
  Wire.write((uint8_t*)&ppm, sizeof(ppm));
  Wire.endTransmission();
  Serial.println("Enviando datos a través de I2C");
}
