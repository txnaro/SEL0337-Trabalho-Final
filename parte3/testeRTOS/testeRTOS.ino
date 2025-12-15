#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define o pino do LED
const int LED_PIN = 2;

//Protótipos das Funções das Tasks
void TaskSerial(void *pvParameters);
void TaskBlink(void *pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Teste FreeRTOS");

  // Cria a Task 1 e fixa no CORE 0
  // xTaskCreatePinnedToCore(função, nome, pilha, parametros, prioridade, handle, coreID)
  xTaskCreatePinnedToCore(
    TaskSerial,   // Função que define a tarefa
    "PrintCore0", // Nome da tarefa
    2048,         // Tamanho da pilha
    NULL,         // Parâmetros
    1,            // Prioridade
    NULL,         // Handle 
    0             // ID do Núcleo
  );

  // Cria a Task 2 e fixa no CORE 1
  xTaskCreatePinnedToCore(
    TaskBlink,   
    "BlinkCore1", 
    2048,        
    NULL,        
    1,            
    NULL,         
    1             
  );
}

void loop() {
  // O loop fica vazio.
  vTaskDelay(pdMS_TO_TICKS(1000));
}

//Implementação da Task 1 (Core 0)
void TaskSerial(void *pvParameters) {
  for(;;) {
    Serial.print("Task Serial rodando no Core: ");
    Serial.println(xPortGetCoreID()); // Deve imprimir "0"
    
    // Bloqueia a tarefa por 1000ms e libera o processador para outras coisas
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

//Implementação da Task 2 (Core 1)
void TaskBlink(void *pvParameters) {
  for(;;) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(500)); // Espera 500ms
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(500)); // Espera 500ms
 
    Serial.print("Task Blink rodando no Core: ");
    Serial.println(xPortGetCoreID()); // Deve imprimir "1"
  }
}