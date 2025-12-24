# Controle de Motor CC via Bluetooth (BLE) com ESP32
![ESP32](https://img.shields.io/badge/ESP32-323330?style=for-the-badge&logo=espressif&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Bluetooth](https://img.shields.io/badge/Bluetooth_BLE-0082FC?style=for-the-badge&logo=bluetooth&logoColor=white)
![RemoteXY](https://img.shields.io/badge/RemoteXY-App_Interface-orange?style=for-the-badge)
![Hardware](https://img.shields.io/badge/Driver-L298N-red?style=for-the-badge)

Este projeto foi desenvolvido para a disciplina de Microcontroladores (Unidade II) com o objetivo de criar um sistema embarcado capaz de controlar remotamente um motor de corrente contínua (DC). O sistema permite ligar/desligar, inverter o sentido de rotação e ajustar a velocidade através de um aplicativo de celular com interface gráfica personalizada.

## Sobre o Projeto

O diferencial deste projeto é a comunicação via **Bluetooth Low Energy (BLE)**, garantindo compatibilidade tanto com Android quanto com iOS (iPhone). A interface de controle foi desenhada e implementada utilizando a plataforma **RemoteXY**, que permite criar dashboards visuais que se comunicam diretamente com a estrutura de dados do microcontrolador.

## Tecnologias e Ferramentas

* **Microcontrolador:** ESP32
* **Driver de Motor:** Ponte H L298N (ou L293D)
* **Comunicação:** Bluetooth Low Energy (BLE)
* **App Móvel:** RemoteXY (Disponível na App Store e Play Store)

## Pinagem e Conexões

O hardware foi configurado para interagir com o módulo de Ponte H da seguinte forma:

| Componente (Ponte H) | Função | Pino do ESP32 | Definição no Código |
| :--- | :--- | :--- | :--- |
| **IN1** | Controle de Direção A | **GPIO 19** | `const int pinoIN1 = 19;` |
| **IN2** | Controle de Direção B | **GPIO 18** | `const int pinoIN2 = 18;` |
| **ENA** | Controle de Velocidade (PWM) | **GPIO 16** | `const int pinoENA = 16;` |

> **Nota:** É necessária uma fonte de alimentação externa adequada para o motor, compartilhando o GND com o ESP32.

## Interface Gráfica (App)

A interface no aplicativo foi configurada para conter os seguintes controles:

1.  **Botão Power:** Liga ou Desliga o sistema (Corte geral).
2.  **Chave de Sentido:** Alterna entre rotação Horária e Anti-Horária.
3.  **Slider (Deslizante):** Ajusta a potência do motor de 0 a 100%.

## Lógica de Funcionamento

### 1. Comunicação BLE e RemoteXY
O código utiliza a biblioteca `RemoteXY.h` em modo BLE (`REMOTEXY_MODE__ESP32CORE_BLE`), permitindo que o iPhone reconheça o ESP32. As variáveis da interface são mapeadas diretamente em uma `struct` no código C++. Quando o usuário move o slider no celular, a variável `RemoteXY.slider_vel` é atualizada automaticamente no ESP32.

### 2. Controle PWM (Velocidade)
O controle de velocidade é feito via PWM (Pulse Width Modulation) no pino **ENA**:
* **Frequência:** 30 kHz (para reduzir ruído audível do motor).
* **Resolução:** 8 bits (valores de 0 a 255).
* **Cálculo:** O slider envia valores de 0 a 100. O código converte isso para a escala do PWM:
    $$PWM = Slider \times 2.55$$

### 3. Controle de Direção (Ponte H)
A lógica da Ponte H é manipulada pelos pinos IN1 e IN2:
* **Sentido 1:** IN1 `HIGH` / IN2 `LOW`
* **Sentido 2:** IN1 `LOW` / IN2 `HIGH`
* **Parar:** Ambos `LOW` ou PWM zerado.

## Como Executar

1.  **Instale o App:** Baixe o **RemoteXY** no seu smartphone.
2.  **Upload:** Carregue o código fornecido no seu ESP32.
3.  **Conexão:**
    * Ative o Bluetooth do celular.
    * Abra o app RemoteXY.
    * Adicione um novo dispositivo via Bluetooth BLE.
    * Conecte-se ao dispositivo nomeado **"ESP32_Motor_Final"**.
4.  **Operação:** A interface gráfica carregará automaticamente na tela do seu celular.

---
*Desenvolvido para a disciplina de Microcontroladores - Unidade II (2025)*
