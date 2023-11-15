#include <Wire.h>
#include <WiFi.h>
#include "ThingSpeak.h"
#include "BluetoothSerial.h"

// Variables para almacenar datos recibidos
float temperatura = 0.0;
float humedad = 0.0;
float co2 = 0.0;

// Inicialización del módulo Bluetooth
BluetoothSerial SerialBT;

// Dirección del esclavo (microcontrolador que envía los datos)
int slaveAddress = 8;

void setup() {
  SerialBT.begin("BluetoothESP32");  // Inicia el módulo Bluetooth con el nombre "BluetoothESP32"

  Wire.begin(8);                    // Inicia la comunicación I2C como esclavo
  Wire.onReceive(receiveEvent);     // Configura la función de evento para la recepción de datos
  Serial.begin(9600);
}

void loop() {
  // Envía datos por Bluetooth cada segundo durante 15 segundos
  for (int i = 15; i > -1; i--) {
    delay(1000);

    // Envía los datos separados por ";" a través de Bluetooth
    SerialBT.print(temperatura);
    SerialBT.print(";");
    SerialBT.print(humedad);
    SerialBT.println(";");
    SerialBT.print(co2);
    SerialBT.println(";");
    SerialBT.println(0);
    SerialBT.println(";");
    SerialBT.println(i);
    SerialBT.println(";");
  }
  
  Serial.println("Esperando 2 segundos...");
  delay(2000);
}

// Función de evento para la recepción de datos I2C
void receiveEvent(int byteCount) {
  if (Wire.available() >= sizeof(float) * 3) {
    // Lee los datos recibidos a través de I2C
    Wire.readBytes((char*)&temperatura, sizeof(temperatura));
    Wire.readBytes((char*)&humedad, sizeof(humedad));
    Wire.readBytes((char*)&co2, sizeof(co2));

  }
}
