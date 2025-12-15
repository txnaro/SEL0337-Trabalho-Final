# SEL0337-Trabalho-Final
Entrega 6 de SEL0337 Por:  
Thiago Ferreira Pires Nusp: 14754762  
Francisco Ygor Grangeiro de Sousa Nusp: 14680979


# Sistema de Monitoramento de Aceleração em Tempo Real com ESP32 e FreeRTOS

**Disciplina:** SEL0337 - Projetos em Sistemas Embarcados  
**Projeto:** Prática 6 - Introdução aos Sistemas Operacionais de Tempo Real (RTOS)

## Sobre o Projeto

Este projeto consiste em um sistema embarcado de tempo real desenvolvido para a aquisição e transmissão de dados inerciais. Utilizando um microcontrolador **ESP32** e o sensor **MPU6050**, o sistema monitora a aceleração nos três eixos (X, Y, Z) e transmite os dados processados via Bluetooth para um dispositivo móvel.

A principal motivação deste experimento foi explorar a arquitetura **Dual-Core** do ESP32 através do **FreeRTOS**, segregando tarefas críticas de aquisição (Hard Real-Time) das tarefas de comunicação (Soft Real-Time) para garantir determinismo e estabilidade.

## Hardware Utilizado

* **Microcontrolador:** ESP32 DevKit V1
* **Sensor:** MPU6050 (Acelerômetro e Giroscópio)
* **Interface:** Smartphone Android (App: Serial Bluetooth Terminal)
* **Comunicação:** Protocolo I2C (Sensor) e Bluetooth Serial SPP (Mobile)

### Pinagem (Pinout)

| Componente | Pino ESP32 | Função |
| :--- | :--- | :--- |
| **MPU6050 VCC** | 3V3 | Alimentação |
| **MPU6050 GND** | GND | Terra |
| **MPU6050 SCL** | GPIO 22 | Clock I2C |
| **MPU6050 SDA** | GPIO 21 | Dados I2C |


![Foto da montagem em protoboard.](https://github.com/user-attachments/assets/26a0a4d3-c94f-4304-818b-be072336e372)

Montagem do circuito em protoboard.

---

## Detalhamento Técnico: Sensor e Comunicação

Esta seção detalha o funcionamento dos periféricos e protocolos utilizados no projeto.

### 1. O Sensor MPU6050
O **MPU6050** é um sensor MEMS (*Micro Electro Mechanical Systems*) que integra um acelerômetro de 3 eixos e um giroscópio de 3 eixos no mesmo silício. Para este projeto, utilizamos apenas o acelerômetro.

* **Princípio de Funcionamento:** O acelerômetro mede a aceleração própria (incluindo a gravidade). Internamente, ele possui massas microscópicas suspensas por molas de silício. Quando o sensor se move, a inércia desloca essas massas, alterando a capacitância entre placas fixas e móveis.
* **Conversão A/D:** O sensor possui conversores Analógico-Digitais (ADC) de 16-bits internos para cada canal. Isso significa que a saída bruta (*Raw Data*) varia de -32768 a +32767.
* **Escala de Sensibilidade:** O sensor foi configurado na escala padrão de **±2g**. Neste intervalo, a sensibilidade é de **16384 LSB/g**. Portanto, para obter o valor em força G, dividimos o valor bruto por 16384.

### 2. Comunicação I2C (Inter-Integrated Circuit)
A comunicação entre o ESP32 (Mestre) e o MPU6050 (Escravo) ocorre via protocolo I2C.

* **Endereçamento:** O MPU6050 responde ao endereço hexadecimal `0x68` (padrão quando o pino AD0 está desconectado ou em GND).
* **Registradores Críticos:**
    * `PWR_MGMT_1 (0x6B)`: Usado para "acordar" o sensor, que inicia em modo *sleep*.
    * `ACCEL_XOUT_H (0x3B)`: Endereço inicial dos dados de aceleração. A leitura é feita em *burst* (sequencial) de 6 bytes (High e Low byte para X, Y e Z).

### 3. Comunicação Bluetooth (Serial Port Profile - SPP)
Para a transmissão de dados, utiliza-se o Bluetooth do ESP32 emulando uma porta serial.
* **Vantagem:** Permite que o ESP32 seja pareado com qualquer smartphone como se fosse um dispositivo serial legado, facilitando o uso de terminais genéricos sem a necessidade de criar um aplicativo Android do zero.

---

## Arquitetura de Software (RTOS)

O firmware foi desenvolvido utilizando o **FreeRTOS**.

### Multicore
O ESP32 possui dois núcleos: **Protocol CPU (Core 0)** e **Application CPU (Core 1)**.

1.  **Task de Aquisição (`TaskLeituraSensor`) -> Core 1**
    * **Prioridade Alta (2):** Garante determinismo na amostragem (20Hz).
    * Calcula: `Aceleração (m/s²) = (Raw / 16384.0) * 9.81`.
    * Envia os dados para a Fila (*Queue*).

2.  **Task de Comunicação (`TaskEnvioBluetooth`) -> Core 0**
    * **Prioridade Baixa (1):** Roda em "background" sem bloquear a leitura.
    * Recebe dados da Fila e formata em CSV: `ax,ay,az`.

### Sincronização (IPC)
Foi utilizada uma **Queue (Fila)** de tamanho 10 para transferir a estrutura de dados `SensorData` entre os núcleos. Isso evita *Race Conditions* (condições de corrida) onde a leitura e a escrita na mesma variável poderiam ocorrer simultaneamente, corrompendo o valor.

---

## Demonstração do funcionamento
Foi gravado um vídeo demonstrando o funcionamento do projeto:

Na pasta "parte3" é possível encontrar todo o código utilizado, em formato .ino e .txt, além de prints, datasheet do sensor e programa de teste do FreeRTOS.

## Fundamentação: RTOS vs. Sistemas de Propósito Geral
O projeto utiliza o **FreeRTOS**, um sistema operacional focado em previsibilidade. Diferente de sistemas como Windows ou Linux, que priorizam o *throughput* (vazão), o RTOS garante que tarefas críticas atendam a prazos temporais (*deadlines*).

### Diferença entre Tasks e Threads/Processos
Conforme solicitado no roteiro da disciplina, destacam-se as diferenças fundamentais aplicadas neste projeto em comparação com práticas anteriores de Linux Embarcado:

1.  **Tasks (FreeRTOS):**
    * São unidades leves de execução que compartilham o espaço de memória do microcontrolador.
    * O escalonamento é estritamente baseado em **prioridades**: a Task de Leitura (Prio 2) sempre interromperá a de Bluetooth (Prio 1) se precisar rodar.
    * Troca de contexto (*context switch*) extremamente rápida.

2.  **Processos/Threads (Linux):**
    * Em sistemas Linux (como na Raspberry Pi), o escalonador (CFS) busca distribuir o tempo de CPU entre processos.
    * Isso pode introduzir *jitter* (variação de tempo), tornando-o menos adequado para leitura de sensores em "hard real-time" sem patches específicos.
