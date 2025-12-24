# Microcontroladores

Este repositório centraliza os projetos práticos desenvolvidos durante a disciplina de **Microcontroladores** (Engenharia da Computação).

O objetivo deste portfólio é demonstrar a progressão no domínio de sistemas embarcados, partindo da arquitetura básica e manipulação de registradores (PIC) até aplicações conectadas de Internet das Coisas (IoT) e controle via Bluetooth (ESP32).

## Tecnologias e Ferramentas

Abaixo estão as principais tecnologias, linguagens e ferramentas utilizadas ao longo de todos os projetos:

### Hardware & Microcontroladores
![PIC](https://img.shields.io/badge/Microchip-PIC-crimson?style=for-the-badge&logo=microchip&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-323330?style=for-the-badge&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino_Hardware-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Raspberry Pi](https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)

### Linguagens de Programação
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)

### IoT, Web & Conectividade
![MQTT](https://img.shields.io/badge/MQTT-3C276D?style=for-the-badge&logo=mqtt&logoColor=white)
![HiveMQ](https://img.shields.io/badge/HiveMQ-f4e11e?style=for-the-badge&logo=hivemq&logoColor=black)
![Bluetooth](https://img.shields.io/badge/Bluetooth_BLE-0082FC?style=for-the-badge&logo=bluetooth&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)

### Softwares e Ferramentas
![CCS C](https://img.shields.io/badge/CCS_C_Compiler-green?style=for-the-badge)
![PICSIMLab](https://img.shields.io/badge/Simulador-PICSIMLab-orange?style=for-the-badge)
![RemoteXY](https://img.shields.io/badge/RemoteXY-App_Interface-orange?style=for-the-badge)


---

## Índice de Projetos

### 1. [Detector de Chuva (PIC16F877A)](https://github.com/ogeorgehenrique/microcontroladores/tree/main/PIC/1.%20Detector%20de%20Chuva%20com%20PIC)
**Foco:** Conversão A/D e Interface Local (LCD).
O primeiro projeto da série. Um sistema stand-alone que utiliza um sensor resistivo para medir a intensidade de chuva e exibe a porcentagem de umidade em um display LCD 16x2.
* **Destaques:** Leitura ADC de 10 bits, multiplexação de LCD, programação em C (CCS).

### 2. [Monitoramento IoT via MQTT (ESP32)](https://github.com/ogeorgehenrique/microcontroladores/tree/main/ESP32/1.%20Detector%20de%20Chuva%20com%20ESP32%20e%20MQTT)
**Foco:** IoT, Protocolo MQTT e Dashboards Web.
Evolução do projeto anterior. Substituímos a exibição local por um envio de dados para a nuvem. O ESP32 coleta os dados do sensor e publica em um Broker MQTT. Um dashboard web (HTML/JS) consome esses dados para gerar gráficos em tempo real.
* **Destaques:** Conexão Wi-Fi, HiveMQ, WebSockets, Biblioteca Highcharts.

### 3. [Laboratório Virtual (PICSIMLab)](https://github.com/ogeorgehenrique/microcontroladores/tree/main/PIC/2.%20Projetos%20com%20PIC)
**Foco:** Simulação, Periféricos e Lógica de Hardware.
Uma coleção de **12 experimentos** simulados nas placas McLab I e II. Abrange desde o "Hello World" (Pisca LED) até controles complexos como PWM, Timers e Comunicação Serial.
* **Destaques:** Projetos funcionais simulados sem necessidade de hardware físico imediato.

### 4. [Controle de Motor via Bluetooth (ESP32)](https://github.com/ogeorgehenrique/microcontroladores/tree/b67841987c2a7bb80415f9f469e074c31d455382/ESP32/2.%20Motor%20Bluetooth.i)
**Foco:** Conectividade BLE, Controle de Potência (PWM) e App Mobile.
Sistema de controle remoto para motores DC. Utiliza o Bluetooth Low Energy do ESP32 para se comunicar com um aplicativo de celular (RemoteXY), permitindo ajuste de velocidade e rotação na palma da mão.
* **Destaques:** Ponte H L298N, PWM de 8 bits, Interface Gráfica Mobile personalizada.

---

## Como Utilizar este Repositório

Cada pasta listada acima contém seu próprio arquivo `README.md` detalhado, o código fonte (`.c`, `.ino`) e os esquemas de ligação necessários. Navegue pelos links do índice para ver os detalhes técnicos de implementação de cada projeto.

---
*Desenvolvido por George Henrique - Estudante de Engenharia da Computação.*
