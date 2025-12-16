# SEL0337 - Trabalho Final
Entrega 6 de SEL0337 Por:  
Thiago Ferreira Pires Número USP: 14754762  
Francisco Ygor Grangeiro de Sousa Número USP: 14680979   

Este repositório contém a documentação e os códigos desenvolvidos para o Projeto Final da disciplina. O trabalho está dividido em duas partes:
1.  **Análise Teórica (Parte 1 do roteiro):** Estudo de caso de um produto comercial (Lâmpada Smart) e validação científica.
2.  **Desenvolvimento Prático (Parte 3 do roteiro):** Implementação de um sistema de monitoramento de aceleração em tempo real utilizando ESP32 e FreeRTOS.


# Parte 1 - Caracterização de Sistemas Embarcados

## 1. Produto Escolhido: Lâmpada Smart Hive Active Light

**Descrição:** O produto analisado é a lâmpada inteligente **Hive Active Light**, um dispositivo de automação residencial controlado remotamente. Diferente de lâmpadas Wi-Fi comuns, este modelo opera através do protocolo **ZigBee**, comunicando-se com um *Hub* central que, por sua vez, conecta-se à internet. Isso permite funcionalidades como ajuste de intensidade, agendamento e integração com assistentes virtuais.

A escolha deste modelo baseou-se na disponibilidade de documentação técnica detalhada de seus componentes internos.

**Fontes de Análise:**
* **Teardown (Análise de Hardware):** [Post na DigiKey - Teardown Hive Smart Bulb](https://www.digikey.ee/en/maker/projects/teardown-hive-smart-bulb/11536204378b4195a03ae8815642a0db)
* **Datasheet do Microcontrolador:** [NXP JN5169 Data Sheet](https://www.nxp.com/docs/en/data-sheet/JN5169-001-M0X-2.pdf)

![lampada utilizada](https://github.com/user-attachments/assets/87c6e31b-313a-4f46-b02c-4bede908cfe7)  
Lâmpada escolhida para a análise. Fonte: All About Circuits.

### Características Técnicas

A arquitetura do sistema é baseada em um SoC otimizado para aplicações de rede sem fio de baixa potência.

| Subsistema | Detalhes Técnicos |
| :--- | :--- |
| **Unidade de Processamento** | **SoC:** NXP JN5169 <br> **Arquitetura:** CPU 32-bit RISC <br> **Clock:** Variável 1 MHz a 32 MHz. |
| **Memória** | Integrada ao SoC: 512 kB Flash, 32 kB RAM e 4 kB EEPROM (para persistência de configurações). |
| **Sistema Operacional** | Pilha de protocolo **ZigBee PRO** rodando sobre *Bare Metal* ou um escalonador de tarefas leve (Ex: JenOS da NXP). |
| **Comunicação Sem Fio** | Padrão **IEEE 802.15.4** (2.4 GHz). <br> Protocolo de Rede: ZigBee 3.0 / ZigBee Home Automation (HA). |
| **Comunicação Com Fio** | Periféricos internos do MCU: I2C, UART, SPI e ADC. |
| **Entradas e Saídas (I/O)** | GPIOs dedicados ao chaveamento do Driver de LED. <br> **PWM:** Modulação por Largura de Pulso para controle preciso de brilho e temperatura de cor. |
| **Sensores e Atuadores** | **Atuador:** Matriz de LEDs acoplada a um driver de potência. |
| **Alimentação** | Fonte AC/DC integrada Buck ou Flyback operando em 220-240V. |
| **Segurança** | Criptografia de Hardware **AES-128** (Nativo do JN5169). |
| **Firmware e Atualizações** | Suporte nativo do padrão ZigBee para atualizações remotas. |
| **Armazenamento Externo** | Não aplicável. |
| **Interface com o Usuário** | Indireta via Aplicativo Móvel. |  


![JN5169 utilizado na lâmpada](https://github.com/user-attachments/assets/99ef87f5-4ad0-4a2d-9ce1-5e575d25e57d)

JN5169 utilizado na lâmpada. Fonte: All About Circuits.

---

## 2. Validação Científica

Abaixo apresentamos a análise de dois artigos científicos que fundamentam as tecnologias e aplicações do produto escolhido.

### Artigo 1: Tecnologias Centrais
**Título:** A Smart Switch to Connect and Disconnect Electrical Devices at Home by Using Internet  
**Referência:** J. E. G. Salas, R. M. Caporal, E. B. Huerta, J. J. Rodriguez and J. J. R. Magdaleno, "A Smart Switch to Connect and Disconnect Electrical Devices at Home by Using Internet," in *IEEE Latin America Transactions*, vol. 14, no. 4, pp. 1575-1581, April 2016. doi: 10.1109/TLA.2016.7483485.

> **Resumo:**
> O artigo apresenta o desenvolvimento de um firmware para um Smart Switch capaz de conectar e desconectar remotamente dispositivos elétricos residenciais por meio da Internet. O sistema utiliza um módulo Wi-Fi com servidor embarcado, permitindo o controle via navegador web tanto em rede local quanto remotamente. São descritos os aspectos de firmware, protocolos de comunicação TCP/IP, comandos AT e a arquitetura de hardware baseada em relé e fontes reguladas. O trabalho também aborda testes experimentais que validam a funcionalidade do sistema, demonstrando sua viabilidade como solução de automação residencial de baixo custo e fácil implementação.

**Análise Crítica:**
* **Questão de Pesquisa:** Como projetar e implementar um dispositivo embarcado simples, de baixo custo e baseado em Wi-Fi, capaz de permitir o controle remoto confiável de cargas elétricas residenciais via Internet ou rede local?
* **Importância:** O problema é altamente relevante para IoT e automação residencial, tratando da democratização do acesso a tecnologias de controle remoto. O trabalho antecipa conceitos de smart homes, como controle via navegador e integração com redes IP, essenciais para LED Smart Switches que exigem baixo consumo e simplicidade.
* **Limitações e Questionamentos:** Os autores não exploram aprofundadamente a segurança (criptografia, autenticação), o que é crítico para dispositivos conectados. O sistema usa comandos simples sem discutir escalabilidade ou OTA. *Pergunta aos autores:* Como o sistema se comportaria com múltiplos dispositivos simultâneos e quais mecanismos de segurança poderiam ser incorporados para garantir proteção contra ataques em redes públicas?

---

### Artigo 2: Aplicações e Estudo de Caso
**Título:** Development of LED smart switch with light-weight middleware for location-aware services in smart home  
**Referência:** Z. Hwang, Y. Uhm, Y. Kim, G. Kim and S. Park, "Development of LED smart switch with light-weight middleware for location-aware services in smart home," in *IEEE Transactions on Consumer Electronics*, vol. 56, no. 3, pp. 1395-1402, Aug. 2010. doi: 10.1109/TCE.2010.5606275.

> **Resumo:**
>O artigo apresenta o desenvolvimento e a implementação de um LED Smart Switch aplicado a ambientes de smart home, com foco em serviços sensíveis à localização do usuário. A proposta integra hardware embarcado, comunicação em rede e um middleware leve, responsável por abstrair dispositivos e facilitar a interoperabilidade. O sistema é validado por meio de um estudo de caso residencial, no qual o acionamento da iluminação LED e ajustes de brilho ocorrem automaticamente conforme a presença do usuário no ambiente e as condições de iluminância. Os resultados demonstram redução no tempo de resposta do sistema, melhoria na experiência do usuário e indicam a viabilidade prática da solução proposta, bem como seu potencial para aplicações com foco em eficiência energética. 

**Análise Crítica:**
* **Questão de Pesquisa:** Como aplicar um LED Smart Switch em um ambiente doméstico real, integrando serviços sensíveis à localização por meio de um middleware leve, sem comprometer o desempenho?
* **Importância:** Relevante para automação e computação ubíqua, abordando a transição para sistemas *context-aware*. Contribui para a compreensão de como sistemas embarcados oferecem eficiência energética e inteligência ambiental. O uso de middleware leve é fundamental para escalabilidade.
* **Limitações e Questionamentos:** O sistema foi validado em pequena escala. Segurança, privacidade de dados de localização e interoperabilidade comercial não são aprofundados. *Pergunta aos autores:* Como o middleware se comportaria com dezenas de dispositivos ativos e como garantir a privacidade dos dados de localização dos usuários em produtos comerciais de larga escala?

---



# Parte 3 - Sistema de Monitoramento de Aceleração em Tempo Real com ESP32 e FreeRTOS
  
**Projeto:** Prática 6 - Introdução aos Sistemas Operacionais de Tempo Real (RTOS)

## Sobre o Projeto

Este projeto consiste em um sistema embarcado de tempo real desenvolvido para a aquisição e transmissão de dados inerciais. Utilizando um microcontrolador ESP32 e o sensor MPU6050, o sistema monitora a aceleração nos três eixos (X, Y, Z) e transmite os dados processados via Bluetooth para um dispositivo móvel.

A principal motivação deste experimento foi explorar a arquitetura Dual-Core do ESP32 através do FreeRTOS, segregando tarefas críticas de aquisição (Hard Real-Time) das tarefas de comunicação (Soft Real-Time) para garantir determinismo e estabilidade.

A implementação deste sensor via barramento I2C adiciona uma camada necessária de complexidade ao projeto, exigindo o gerenciamento preciso de endereçamento e timing de comunicação. Além disso, a necessidade de converter os dados brutos dos registradores internos em grandezas físicas utilizáveis cria uma carga de processamento representativa, validando a eficiência do escalonamento de tarefas proposto.
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
Foi utilizada uma Queue de tamanho 10 para transferir a estrutura de dados `SensorData` entre os núcleos. Isso evita Race Conditions onde a leitura e a escrita na mesma variável poderiam ocorrer simultaneamente, corrompendo o valor.

---

## Demonstração do funcionamento
Foi gravado um vídeo demonstrando o funcionamento do projeto:  
https://youtu.be/_0PJqAqvBnI

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
