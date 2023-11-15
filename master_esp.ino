#include <Wire.h>
#include <WiFi.h>
#include "ThingSpeak.h"

// Configuración de la red WiFi
const char* ssid = "UNICABLE_ORTEGA_LOPEZ";  // Nombre de la red WiFi
const char* password = "39034937";           // Contraseña de la red WiFi

WiFiClient client;

// Configuración de ThingSpeak
unsigned long myChannelNumber = 3;
const char* myWriteAPIKey = "ZL3OO4Q5GYOAMIVO";

// Variables para almacenar datos recibidos
float temperatura = 0.0;
float humedad = 0.0;
float co2 = 0.0;

// Dirección del esclavo (microcontrolador que envía los datos)
int slaveAddress = 8;

// Variables del temporizador
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;  // Intervalo de tiempo entre actualizaciones

void setup() {
  Wire.begin(8);                // Inicia la comunicación I2C como esclavo
  Wire.onReceive(receiveEvent); // Configura la función de evento para la recepción de datos
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);      // Inicializa ThingSpeak
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

    // Establece los campos con los valores recibidos
    ThingSpeak.setField(1, temperatura);
    ThingSpeak.setField(2, humedad);
    ThingSpeak.setField(3, co2);

    // Escribe en el canal de ThingSpeak
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Actualización exitosa del canal.");
      Serial.print("Data1: ");
      Serial.println(temperatura);
      Serial.print("Data2: ");
      Serial.println(humedad);
      Serial.print("Data3: ");
      Serial.println(co2);
    } else {
      Serial.println("Problema al actualizar el canal. Código de error HTTP " + String(x));
    }
    lastTime = millis(); // Actualiza el tiempo del último envío
  }
}

// Función de evento para la recepción de datos I2C
void receiveEvent(int byteCount) {
  if (Wire.available() >= sizeof(float) * 3) {
    Wire.readBytes((char*)&temperatura, sizeof(temperatura));
    Wire.readBytes((char*)&humedad, sizeof(humedad));
    Wire.readBytes((char*)&co2, sizeof(co2));

  }
}
