# SEL0337 - Trabalho Final
Entrega 6 de SEL0337 Por:  
Thiago Ferreira Pires Número USP: 14754762  
Francisco Ygor Grangeiro de Sousa Número USP: 14680979   

Este repositório contém a documentação e os códigos desenvolvidos para o Projeto Final da disciplina. O trabalho está dividido em duas partes:
1.  **Análise Teórica (Parte 1 do roteiro):** Estudo de caso de um produto comercial (Lâmpada Smart/Smart Switch) e validação científica.
2.  **Desenvolvimento Prático (Parte 3 do roteiro):** Implementação de um sistema de monitoramento inercial em tempo real utilizando ESP32 e FreeRTOS.


# Parte 1 - Caracterização de Sistemas Embarcados
## 1. Produto Escolhido: Lâmpada Smart Hive Smart Bulb

**Descrição:** O produto analisado é uma Lâmpada Inteligente (Smart Bulb) voltado para automação residencial. O dispositivo permite o controle remoto da iluminação via Wi-Fi, ajuste de intensidade, programação de horários e integração com assistentes virtuais a partir de um aplicativo móvel. Escolhemos o modelo da Hive pois foi possível encontrar um post *on-line* na *DigiKey* (fornecedor de componentes eletrônicos) sobre seus componentes internos.  
O post pode ser acessado em: https://www.digikey.ee/en/maker/projects/teardown-hive-smart-bulb/11536204378b4195a03ae8815642a0db  

Outra fonte de dados é o *datasheet* do MCU utilizado na lâmpada (JN5169), que pode ser acessado na pasta da parte 1 ou através do link:
https://www.nxp.com/docs/en/data-sheet/JN5169-001-M0X-2.pdf

### Características Técnicas


| Subsistema | Detalhes Técnicos |
| :--- | :--- |
| **Unidade de Processamento** | MCU: NXP JN5169, Arquitetura: 32 Bits RISC, Fabricante: NXP, Clock: 1MHz até 32MHz |
| **Memória** | O JN5169 possui 512Kb de Flash, 32kB de RAM e 4kB de EEPROM |
| **Sistema Operacional** | Não é possível afirmar, mas por ser um MCU será bare metal ou RTOS |
| **Comunicação Sem Fio** | 2.4 GHz IEEE 802.15.4, ZigBee 3.0 e ZigBee PRO |
| **Comunicação Com Fio** | O MCU possui I2C, UART, SPI e ADC|
| **Entradas e Saídas (I/O)** | GPIOs para controle do LED <br> PWM: Modulação para controle de brilho/cor |
| **Sensores e Atuadores** | **Atuadores:** Driver de LED. <br> **Sensores:** Sensor de corrente/tensão. <br> Não foi possível determinar a comunicação na aplicação.|
| **Alimentação** | Fonte AC/DC integrada 220-240V. |
| **Segurança** | AES-128 |
| **Firmware e Atualizações** | Não foi possível determinar. |
| **Armazenamento externo** | Não se aplica. |
| **Interface com o Usuário** | Aplicativo móvel via rede sem fio. |

Foi possível determinar grande parte das caracteristicas do sistema, com exceção da comunicação que faz o controle dos LEDs.  
Quanto ao controle dos LEDs, por se tratar de um driver chaveado com controlador próprio, acreditamos que a cominicação é feita através de PWM.  

---

## 2. Validação Científica

Abaixo apresentamos a análise de dois artigos científicos que fundamentam as tecnologias e aplicações do produto escolhido.

### 📄 Artigo 1: Tecnologias Centrais
**Título:** A Smart Switch to Connect and Disconnect Electrical Devices at Home by Using Internet  
**Referência:** J. E. G. Salas, R. M. Caporal, E. B. Huerta, J. J. Rodriguez and J. J. R. Magdaleno, "A Smart Switch to Connect and Disconnect Electrical Devices at Home by Using Internet," in *IEEE Latin America Transactions*, vol. 14, no. 4, pp. 1575-1581, April 2016. doi: 10.1109/TLA.2016.7483485.

> **Resumo:**
> O artigo apresenta o desenvolvimento de um firmware para um Smart Switch capaz de conectar e desconectar remotamente dispositivos elétricos residenciais por meio da Internet. O sistema utiliza um módulo Wi-Fi com servidor embarcado, permitindo o controle via navegador web tanto em rede local quanto remotamente. São descritos os aspectos de firmware, protocolos de comunicação TCP/IP, comandos AT e a arquitetura de hardware baseada em relé e fontes reguladas. O trabalho também aborda testes experimentais que validam a funcionalidade do sistema, demonstrando sua viabilidade como solução de automação residencial de baixo custo e fácil implementação.

**Análise Crítica:**
* **Questão de Pesquisa:** Como projetar e implementar um dispositivo embarcado simples, de baixo custo e baseado em Wi-Fi, capaz de permitir o controle remoto confiável de cargas elétricas residenciais via Internet ou rede local?
* **Importância:** O problema é altamente relevante para IoT e automação residencial, tratando da democratização do acesso a tecnologias de controle remoto. O trabalho antecipa conceitos de smart homes, como controle via navegador e integração com redes IP, essenciais para LED Smart Switches que exigem baixo consumo e simplicidade.
* **Limitações e Questionamentos:** Os autores não exploram aprofundadamente a segurança (criptografia, autenticação), o que é crítico para dispositivos conectados. O sistema usa comandos simples sem discutir escalabilidade ou OTA. *Pergunta aos autores:* Como o sistema se comportaria com múltiplos dispositivos simultâneos e quais mecanismos de segurança poderiam ser incorporados para garantir proteção contra ataques em redes públicas?

---

### 📄 Artigo 2: Aplicações e Estudo de Caso
**Título:** Development of LED smart switch with light-weight middleware for location-aware services in smart home  
**Referência:** Z. Hwang, Y. Uhm, Y. Kim, G. Kim and S. Park, "Development of LED smart switch with light-weight middleware for location-aware services in smart home," in *IEEE Transactions on Consumer Electronics*, vol. 56, no. 3, pp. 1395-1402, Aug. 2010. doi: 10.1109/TCE.2010.5606275.

> **Resumo:**
> O artigo apresenta o desenvolvimento de um LED Smart Switch aplicado a ambientes de smart home, com foco em serviços sensíveis à localização. A proposta integra hardware embarcado, rede e um middleware leve para interoperabilidade. O sistema é validado em um estudo de caso residencial, onde a iluminação LED e o brilho se ajustam automaticamente conforme a presença do usuário. Os resultados demonstram redução no tempo de resposta, melhoria na experiência do usuário e viabilidade prática para eficiência energética.

**Análise Crítica:**
* **Questão de Pesquisa:** Como aplicar um LED Smart Switch em um ambiente doméstico real, integrando serviços sensíveis à localização por meio de um middleware leve, sem comprometer o desempenho?
* **Importância:** Relevante para automação e computação ubíqua, abordando a transição para sistemas *context-aware*. Contribui para a compreensão de como sistemas embarcados oferecem eficiência energética e inteligência ambiental. O uso de middleware leve é fundamental para escalabilidade.
* **Limitações e Questionamentos:** O sistema foi validado em pequena escala. Segurança, privacidade de dados de localização e interoperabilidade comercial não são aprofundados. *Pergunta aos autores:* Como o middleware se comportaria com dezenas de dispositivos ativos e como garantir a privacidade dos dados de localização dos usuários em produtos comerciais de larga escala?

---



# Parte 3 - Sistema de Monitoramento de Aceleração em Tempo Real com ESP32 e FreeRTOS
  
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
