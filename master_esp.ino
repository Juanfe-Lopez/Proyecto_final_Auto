#include <Wire.h>
#include <WiFi.h>
#include "ThingSpeak.h"


const char* ssid = "UNICABLE_ORTEGA_LOPEZ";  // tu SSID de la red
const char* password = "39034937";           // tu contraseña de red

WiFiClient client;

unsigned long myChannelNumber = 3;
const char* myWriteAPIKey = "ZL3OO4Q5GYOAMIVO";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

float temperatura = 0.0;
float humedad = 0.0;
float co2 = 0.0;

int slaveAddress = 8;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Inicializa ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {

    // Conectar o reconectar a WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Intentando conectar");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(5000);
      }
      Serial.println("\nConectado.");
    }


    // Set the fields with the values
    ThingSpeak.setField(1, temperatura);
    ThingSpeak.setField(2, humedad);
    ThingSpeak.setField(3, co2);

    // Write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Channel update successful.");
      Serial.print("Data1: ");
      Serial.println(temperatura);
      Serial.print("Data2: ");
      Serial.println(humedad);
      Serial.print("Data3: ");
      Serial.println(co2);
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
}
void receiveEvent(int byteCount) {
  if (Wire.available() >= sizeof(float) * 3) {
    Wire.readBytes((char*)&temperatura, sizeof(temperatura));
    Wire.readBytes((char*)&humedad, sizeof(humedad));
    Wire.readBytes((char*)&co2, sizeof(co2));

    // Puedes imprimir los datos recibidos para verificar en el monitor serial
  }
}
