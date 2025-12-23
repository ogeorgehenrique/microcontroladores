# Projeto: Sensor Detector de Chuva com PIC16F877A

Este projeto faz parte de uma série de estudos para a disciplina de microcontroladores. O objetivo é desenvolver um sistema de monitoramento ambiental capaz de detectar a presença de água/chuva utilizando um sensor analógico/digital e exibir os dados processados em um display LCD.

## Sobre o Projeto

O sistema lê o valor de tensão fornecido por um módulo sensor de chuva através do conversor Analógico-Digital (ADC) do microcontrolador `PIC16F877A`. O valor lido é convertido em uma porcentagem que representa o nível de "seca" (ausência de umidade) e exibido em tempo real em um LCD. LEDs são utilizados para indicação visual de funcionamento do ciclo de leitura.

## Hardware Utilizado

* **Microcontrolador:** PIC16F877A
* **Sensor:** Módulo Sensor de Chuva (Saída Analógica)
* **Display:** LCD 16x2 (Hitachi HD44780 controller ou compatível)
* **Potenciomentro de 1K**

### Componentes para o funcionamento do PIC
* **Oscilador:** Cristal de 20MHz (High Speed)
* **Diodo Rápido**
* **Push Botton**
* **2 capacitores 30pF (Cerâmico)**
* **Mini Jumpes**
* **Barra de Pinos Macho**
* **2 Resistores 10KΩ**
* **4 Resistores 330KΩ**
* **Fonte Ajustavel para protoboard**
* **Protoboard 830 pinos**

### Componentes Extras:
* 2x LEDs (Indicadores de status)
* Resistores e Jumpers

## Pinagem e Conexões

As conexões foram definidas via software da seguinte maneira:

| Componente | Pino do Componente | Pino do PIC (Porta) | Definição no Código |
| :--- | :--- | :--- | :--- |
| **Sensor Chuva** | Saída Analógica (AO) | **RA0 / AN0** | `setup_adc_ports(AN0)` |
| **LCD** | Enable (E) | **RC1** | `pin_c1` |
| **LCD** | RS | **RC0** | `pin_c0` |
| **LCD** | Dados (D4) | **RD4** | `pin_d4` |
| **LCD** | Dados (D5) | **RD5** | `pin_d5` |
| **LCD** | Dados (D6) | **RD6** | `pin_d6` |
| **LCD** | Dados (D7) | **RD7** | `pin_d7` |
| **LED 1** | Anodo (+) | **RD1** | `PIN_D1` |
| **LED 2** | Anodo (+) | **RD2** | `PIN_D2` |

> **Nota:** Os pinos RW do LCD geralmente são aterrados (GND) se não definidos no código para escrita.

## Imagem do Projeto montado:
![IMG_4700](https://github.com/user-attachments/assets/ff29ac50-baea-4749-bf7f-b950e5c482dc)

![IMG_4699](https://github.com/user-attachments/assets/876b4bdc-08e4-4500-852c-5b702e1d2ab8)


## Software e Bibliotecas

* **Linguagem:** C (Compilador CCS C)
* **Bibliotecas:**
    * `<16F877A.h>`: Definições do microcontrolador.
    * `"mod_lcd.c"`: Driver customizado para controle do LCD (deve estar na mesma pasta do projeto).

## Lógica de Funcionamento

1.  **Inicialização:**
    * Configura o ADC para resolução de 10 bits (`#device ADC=10`).
    * Inicializa o LCD e exibe a mensagem de boas-vindas.
2.  **Loop Principal:**
    * **Sinalização:** Pisca os LEDs conectados em D1 e D2 sequencialmente para indicar que o sistema está ativo.
    * **Leitura:** O canal analógico 0 (AN0) é lido. O valor varia de 0 a 1023 (10 bits).
    * **Cálculo:** O código converte a leitura bruta em porcentagem:
        $$x = \frac{valor \times 100}{1023}$$
    * **Exibição:** O LCD mostra a mensagem "Seco: X%", indicando a porcentagem da leitura atual.

## Observações Técnicas

* **Interpretação do Sensor:** Sensores de chuva resistivos típicos funcionam como divisores de tensão.
    * *Seco:* Alta resistência -> Tensão próxima a 5V (Leitura ~1023).
    * *Molhado:* Baixa resistência -> Tensão cai para 0V (Leitura ~0).
    * Por isso, o cálculo percentual no código mostra 100% quando está totalmente seco e cai para 0% quando está totalmente submerso/molhado.

---
*Desenvolvido durante a diciplina de Microcontoladores na Engenharia da Computação - IFMT.*
