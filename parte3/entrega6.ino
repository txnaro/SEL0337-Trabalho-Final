#include <Arduino.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Endereço I2C do MPU6050
const int MPU_ADDR = 0x68;

// Constante da gravidade para conversão
const float gravidade = 9.81; 

// Objeto Bluetooth
BluetoothSerial SerialBT;

// Estrutura dos dados do sensor
struct SensorData {
  float accX;
  float accY;
  float accZ;
};

// Handle da Fila
QueueHandle_t sensorQueue;

// Protótipos das Tasks
void TaskLeituraSensor(void *pvParameters);
void TaskEnvioBluetooth(void *pvParameters);

void setup() {
  Serial.begin(115200);

  // Inicializa Bluetooth
  if (!SerialBT.begin("ESP32_Pratica6")) {
    Serial.println("Erro no bluetooth.");
    while(1);
  }
  Serial.println("Bluetooth Conectado.");
  
  // Inicializa I2C
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Registrador PWR_MGMT_1
  Wire.write(0);    // Acorda o MPU-6050
  Wire.endTransmission(true);

  // Cria fila para 10 itens do tipo SensorData
  sensorQueue = xQueueCreate(10, sizeof(SensorData));

  // Task 1: Leitura (Alta Prioridade - Core 1)
  xTaskCreatePinnedToCore(TaskLeituraSensor, "LeituraMPU", 4096, NULL, 2, NULL, 1);

  // Task 2: Bluetooth (Baixa Prioridade - Core 0)
  xTaskCreatePinnedToCore(TaskEnvioBluetooth, "EnvioBT", 4096, NULL, 1, NULL, 0);
}

void loop() {
  vTaskDelete(NULL);
}

// TASK 1: Lê o sensor e converte para m/s^2

void TaskLeituraSensor(void *pvParameters) {
  SensorData data;
  int16_t rawX, rawY, rawZ;
  
  for(;;) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);

    if (Wire.available() == 6) {
      // 1. Lê os dados brutos (Raw - Int16)
      rawX = (Wire.read() << 8 | Wire.read());
      rawY = (Wire.read() << 8 | Wire.read());
      rawZ = (Wire.read() << 8 | Wire.read());

      // 2. Converte para m/s^2
      data.accX = (rawX / 16384.0) * gravidade;
      data.accY = (rawY / 16384.0) * gravidade;
      data.accZ = (rawZ / 16384.0) * gravidade;

      // Envia dados processados para a fila
      xQueueSend(sensorQueue, &data, portMAX_DELAY);
    }
    
    // Taxa de amostragem de ~20Hz (50ms)
    vTaskDelay(pdMS_TO_TICKS(50)); 
  }
}

// TASK 2: Envia para o celular
void TaskEnvioBluetooth(void *pvParameters) {
  SensorData receivedData;

  for(;;) {
    if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY) == pdTRUE) {
      
      if (SerialBT.hasClient()) {

        SerialBT.print(receivedData.accX, 2); 
        SerialBT.print(",");
        SerialBT.print(receivedData.accY, 2);
        SerialBT.print(",");
        SerialBT.println(receivedData.accZ, 2); 
      }
    }
  }
}