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





---

##  Arquitetura do Firmware (RTOS)

O software foi estruturado seguindo o modelo produtor-consumidor, maximizando o uso dos dois núcleos do ESP32.

### Tabela de Tarefas (Tasks)

| Task | Núcleo (Core) | Prioridade | Descrição |
| :--- | :--- | :--- | :--- |
| `TaskLeituraSensor` | **Core 1** (App) | **Alta (2)** | Realiza a leitura I2C a cada 50ms, converte os dados brutos para $m/s^2$ e envia para a fila. |
| `TaskEnvioBluetooth`| **Core 0** (Pro) | **Baixa (1)** | Consome os dados da fila, formata em CSV e transmite via Bluetooth. |

### Comunicação Inter-Processos (IPC)
Utiliza-se uma **Fila (Queue)** do FreeRTOS para transferir a estrutura de dados (`SensorData`) entre o Core 1 e o Core 0. Isso desacopla a amostragem da transmissão, evitando condições de corrida (*Race Conditions*).

---

##  Documentação Técnica

### Fundamentação: RTOS vs. Sistemas de Propósito Geral
O projeto utiliza o **FreeRTOS**, um sistema operacional focado em previsibilidade. Diferente de sistemas como Windows ou Linux, que priorizam o *throughput* (vazão), o RTOS garante que tarefas críticas atendam a prazos temporais (*deadlines*).

### Diferença entre Tasks e Threads/Processos
Conforme solicitado no roteiro da disciplina, destacam-se as diferenças fundamentais aplicadas neste projeto em comparação com práticas anteriores de Linux Embarcado:

1.  **Tasks (FreeRTOS):**
    * São unidades leves de execução que compartilham o espaço de memória do microcontrolador.
    * O escalonamento é estritamente baseado em **prioridades**: a Task de Leitura (Prio 2) sempre interromperá a de Bluetooth (Prio 1) se precisar rodar.
    * Troca de contexto (*context switch*) extremamente rápida.

2.  **Processos/Threads (Linux):**
    * Em sistemas Linux (como na Raspberry Pi), o escalonador (CFS) busca "justiça" na distribuição de tempo de CPU entre processos.
    * Isso pode introduzir *jitter* (variação de tempo), tornando-o menos adequado para leitura de sensores em "hard real-time" sem patches específicos (PREEMPT_RT).
