#include <Wire.h>
#include <WiFi.h>
#include "ThingSpeak.h"
#include "BluetoothSerial.h"



float temperatura = 0.0;
float humedad = 0.0;
float co2 = 0.0;


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

int slaveAddress = 8;

void setup() {
  SerialBT.begin("BluetoothESP32");

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  for (int i = 15; i > -1; i--) {
    delay(1000);

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
void receiveEvent(int byteCount) {
  if (Wire.available() >= sizeof(float) * 3) {
    Wire.readBytes((char*)&temperatura, sizeof(temperatura));
    Wire.readBytes((char*)&humedad, sizeof(humedad));
    Wire.readBytes((char*)&co2, sizeof(co2));

    // Puedes imprimir los datos recibidos para verificar en el monitor serial
  }
}
