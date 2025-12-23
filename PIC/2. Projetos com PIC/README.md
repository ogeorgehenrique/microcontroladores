# Projetos Virtuais de Microcontroladores com PICSIMLab
![PIC](https://img.shields.io/badge/Microchip-PIC-crimson?style=for-the-badge&logo=microchip&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![PICSIMLab](https://img.shields.io/badge/Simulador-PICSIMLab-orange?style=for-the-badge)
![CCS C](https://img.shields.io/badge/Compilador-CCS_C-green?style=for-the-badge)

Este repositório reúne uma coleção de **12 projetos práticos** desenvolvidos durante a disciplina de Microcontroladores. Todos os experimentos foram simulados no ambiente **PICSIMLab** (PIC Simulator Laboratory), utilizando as placas virtuais de desenvolvimento **McLab I** e **McLab II**.

O objetivo foi explorar desde o controle básico de GPIOs até o uso de periféricos avançados como ADC, PWM, Timers e comunicação Serial.

## Tecnologias e Ferramentas

* **Simulador:** PICSIMLab (v0.8 ou superior)
* **Microcontroladores:** PIC16F877A e PIC16F628A
* **Placas Virtuais:** McLab I e McLab II
* **Linguagem:** C (Compilador CCS)

## Sobre o PICSIMLab e as Placas

O **PICSIMLab** é um emulador de placas de desenvolvimento em tempo real com depurador MPLAB X/avr-gdb integrado. Ele permite testar códigos em um ambiente seguro antes da implementação em hardware físico.

* **McLab I:** Placa didática focada no PIC16F628A, ideal para projetos compactos e testes de lógica básica.
* **McLab II:** Placa mais robusta para o PIC16F877A, contendo periféricos como LCD, Displays de 7 segmentos, Matriz de LEDs e interfaces de motor.

## Lista de Projetos

Abaixo estão listados os 12 experimentos funcionais contidos neste portfólio, ordenados por complexidade:

### 1. Básico (GPIO e Lógica)
* **01 - Pisca-Pisca LED:** O "Hello World" do hardware. Controle de temporização e acionamento de saídas digitais.
* **02 - Botão e LED:** Leitura de entradas digitais (botões) para controle condicional de saídas.
* **03 - Semáforo:** Simulação de um cruzamento utilizando lógica sequencial e temporizadores (delay).

### 2. Displays e Interface Homem-Máquina (IHM)
* **04 - Display de 7 Segmentos:** Multiplexação e exibição de contadores numéricos.
* **05 - LCD "Olá Mundo":** Inicialização e escrita de caracteres em displays de cristal líquido (16x2).
* **06 - LCD + Conversor AD:** Leitura de sensores analógicos e exibição dos valores processados no LCD.

### 3. Temporização e PWM (Pulse Width Modulation)
* **07 - Timer Zero:** Configuração de interrupções de tempo precisas usando o Timer0 interno.
* **08 - PWM Cooler (Potenciômetro):** Controle de velocidade de uma ventoinha variando o *Duty Cycle* via entrada analógica.
* **09 - PWM Cooler (Fade-in):** Algoritmo de suavização para acelerar a ventoinha gradualmente (rampa de aceleração).

### 4. Controle de Motores e Atuadores
* **10 - Motor de Passo:** Acionamento sequencial de bobinas para controle preciso de posição.
* **11 - Servo Motor:** Controle de ângulo de eixo utilizando pulsos PWM específicos (geralmente 50Hz).

### 5. Comunicação
* **12 - Comunicação Serial (UART):** Transmissão e recepção de dados entre o microcontrolador e um terminal virtual (ex: Computador).

## Como Executar

Para testar qualquer um dos projetos:
1.  Tenha o **PICSIMLab** instalado.
2.  Carregue o arquivo `.hex` correspondente ao projeto desejado.
3.  Selecione a placa correta (**McLab I** para 16F628A ou **McLab II** para 16F877A) no menu do simulador.
4.  Ative a simulação.

---
*Projetos desenvolvidos para a disciplina de Microcontroladores.*
