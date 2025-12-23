# Projeto: Detector de Chuva IoT com ESP32 e MQTT

![ESP32](https://img.shields.io/badge/ESP32-323330?style=for-the-badge&logo=espressif&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Arduino IDE](https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![MQTT](https://img.shields.io/badge/MQTT-3C276D?style=for-the-badge&logo=mqtt&logoColor=white)
![HiveMQ](https://img.shields.io/badge/HiveMQ-f4e11e?style=for-the-badge&logo=hivemq&logoColor=black)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)
![Highcharts](https://img.shields.io/badge/Highcharts-46465C?style=for-the-badge&logo=highcharts&logoColor=white)

Este projeto expande os estudos de microcontroladores para a área de IoT (Internet das Coisas). O objetivo foi criar um sistema conectado que monitora níveis de chuva e envia os dados em tempo real para um Dashboard Web via protocolo MQTT.

## Sobre o Projeto

Diferente do projeto anterior com PIC (que exibia dados localmente), este sistema utiliza o **ESP32** para conectar-se à rede Wi-Fi. Ele lê um sensor de chuva analógico e publica os dados em um **Broker MQTT** público (HiveMQ).

Uma página web (`dashboard.html`) atua como cliente, assinando o tópico para receber os dados e plotar um gráfico dinâmico em tempo real utilizando a biblioteca Highcharts.

## Hardware Utilizado

* **Microcontrolador:** ESP32 (DevKit V1)
* **Sensor:** Módulo Sensor de Chuva (Saída Analógica)
* **Atuador:** LED (Para feedback remoto)
* **Conectividade:** Wi-Fi 2.4GHz

# Imagem da montagem do Circuito

![IMG_4702](https://github.com/user-attachments/assets/d39f67aa-a351-4ae5-a782-e1c6750bce25)


## Pinagem e Conexões

As conexões físicas definidas no firmware são:

| Componente | Pino do Componente | Pino do ESP32 (GPIO) | Definição no Código |
| :--- | :--- | :--- | :--- |
| **Sensor Chuva** | Saída Analógica (AO) | **GPIO 36 (VP)** | `const int pinoSensor = 36;`|
| **LED Status** | Anodo (+) | **GPIO 26** | `const int pinoLED = 26;` |

## Arquitetura MQTT

O sistema utiliza o protocolo MQTT (Message Queuing Telemetry Transport) com a seguinte configuração:

* **Broker:** `broker.hivemq.com`
* **Porta ESP32 (TCP):** `1883`
* **Porta Dashboard (WebSocket):** `8000` (Usado pelo JavaScript/Paho)
* **Tópicos:**
    * **Publicação (Envio):** `george/sensor/chuva` (Dados do sensor)
    * **Subscrição (Comando):** `george/sensor/led` (Controle do LED)

## Funcionamento do Firmware (ESP32)

1.  **Conexão:** O ESP32 conecta-se ao Wi-Fi ("GEORGE") e em seguida ao Broker MQTT.
2.  **Leitura e Tratamento:**
    * O ADC do ESP32 tem resolução de 12 bits (0 a 4095).
    * Como o sensor de chuva é resistivo (tensão cai quando molha), o código inverte a lógica para tornar a leitura intuitiva (maior valor = mais chuva):
        $$NivelChuva = 4095 - LeituraAnalogica$$
3.  **Envio de Dados:** A cada **2 segundos** (definido por `MSG_INTERVAL`), o ESP32 publica o valor tratado no tópico `george/sensor/chuva`.
4.  **Controle Remoto:** O ESP32 fica escutando o tópico `george/sensor/led`. Se receber `'1'`, liga o LED; se receber `'0'`, desliga.

## Funcionamento do Dashboard (Web)

O arquivo `dashboard.html` roda no navegador e funciona da seguinte forma:

1.  **WebSockets:** Conecta ao mesmo Broker HiveMQ usando a porta **8000** (suporte a MQTT via WebSocket).
2.  **Gráfico em Tempo Real:** Utiliza a biblioteca **Highcharts**.
3.  **Atualização:** Sempre que uma nova mensagem chega no tópico, a função `onMessageArrived` converte o texto para número e adiciona um ponto ao gráfico, mantendo um histórico visual dos últimos 20 pontos de leitura.

## Como Executar

1. Carregue o código `.ino` no seu ESP32.
2. Conecte no `broker.hivemq.com` com as informçoes contidas dentro do código `.ino`
3. Abra o arquivo `dashboard.html` em qualquer navegador moderno.
4.  Assim que o ESP32 conectar (LEDs do módulo podem indicar), o status no site mudará para "Conectado" e o gráfico começará a ser desenhado.


---
*Desenvolvido durante a disciplina de Microcontroladores - Engenharia da Computação.*
