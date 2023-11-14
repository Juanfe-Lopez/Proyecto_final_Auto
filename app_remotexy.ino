// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <Wire.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY"


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 169 bytes
  { 255,0,0,63,0,162,0,16,31,1,71,56,9,17,15,15,0,2,24,135,
  0,0,0,0,0,0,200,66,0,0,160,65,0,0,32,65,0,0,0,64,
  24,0,66,1,48,15,7,16,2,26,67,4,12,67,20,5,2,26,11,72,
  12,26,50,12,12,2,26,140,38,0,0,0,0,0,0,250,67,0,0,0,
  0,67,4,34,67,20,5,2,26,11,69,0,28,77,10,10,1,129,0,5,
  4,23,4,17,84,101,109,112,101,114,97,116,117,114,97,0,129,0,42,5,
  17,4,17,72,117,109,101,100,97,100,0,129,0,28,35,8,4,17,67,79,
  50,0,67,4,6,10,20,5,2,26,11,67,4,42,9,20,5,2,26,11,
  67,4,22,42,20,5,2,26,11 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // output variables
  float Temperatura;  // from 0 to 100 
  int8_t humedad; // =0..100 level position 
  char timer[11];  // string UTF8 end zero 
  int8_t co2;  // from 0 to 500 
  char timer1[11];  // string UTF8 end zero 
  int16_t sound_1; // =0 no sound, else ID of sound, =1001 for example, look sound list in app 
  char temp[11];  // string UTF8 end zero 
  char hum[11];  // string UTF8 end zero 
  char text_co2[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

float temperatura = 0.0;
float humedad = 0.0;
float co2 = 0.0;



void setup() {
  RemoteXY_Init();
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  RemoteXY_Handler();
  for (int i = 15; i > -1; i--) {
    dtostrf(0, 0, 1, RemoteXY.timer);
    dtostrf(i, 0, 1, RemoteXY.timer1);
    dtostrf(temperatura, 0, 1, RemoteXY.temp);
    dtostrf(humedad, 0, 1, RemoteXY.hum);
    dtostrf(co2, 0, 1, RemoteXY.text_co2);
    RemoteXY.humedad = humedad;
    RemoteXY.Temperatura = temperatura;
    RemoteXY.co2 = co2;
    if (i == 0) {
      RemoteXY.sound_1 = 2001;
    } else {
      RemoteXY.sound_1 = 0;
    }

    RemoteXY_delay(1000);
  }
  Serial.println("Esperando 2 segundos...");
  RemoteXY_delay(2000);

}
void receiveEvent(int byteCount) {
  if (Wire.available() >= sizeof(float) * 3) {
    Wire.readBytes((char*)&temperatura, sizeof(temperatura));
    Wire.readBytes((char*)&humedad, sizeof(humedad));
    Wire.readBytes((char*)&co2, sizeof(co2));

    // Puedes imprimir los datos recibidos para verificar en el monitor serial
  }
}
