/*
 * ==============================================================================
 * PROJETO: Contador Ping-Pong (25 a 60)
 * PLACA ALVO: McLab 1 (PICSIMLab)
 * MICROCONTROLADOR: PIC16F628A
 *
 * DESCRIÇÃO:
 * Este código implementa um contador que é exibido nos dois displays
 * de 7 segmentos da placa McLab 1.
 * O contador começa em 25, sobe até 60, depois desce até 25,
 * e repete esse ciclo "ping-pong" indefinidamente.
 * A mudança de número ocorre a cada 1 segundo.
 *
 * ATENÇÃO: Este código foi feito especificamente para a McLab 1.
 * Ele NÃO funcionará na McLab 2, pois ela usa um PIC diferente (16F877A)
 * e tem um hardware (pinagem) diferente.
 * ==============================================================================
 */

// --- Inclusão de Bibliotecas ---
#include <16F628A.h> // Importa o "manual" do PIC16F628A, informando ao compilador
                     // quais são os pinos, registradores, etc.

// --- Configurações do Microcontrolador (FUSES) ---
// Fuses são "configurações de hardware" gravadas no chip.
#FUSES NOWDT         // Desabilita o "Watch Dog Timer" (Cão de Guarda).
                     // (Se o PIC travasse, o WDT o reiniciaria. Não precisamos disso aqui.)
#FUSES NOBROWNOUT   // Desabilita o "Brownout Reset".
                     // (Que reiniciaria o PIC se a tensão de alimentação caísse muito.)
#FUSES NOLVP        // Desabilita o "Low Voltage Programming" (Programação em Baixa Tensão).
                     // Isso é importante pois libera o pino RB3 para uso normal.

// --- Configuração do Clock (Velocidade) ---
#use delay(crystal=20000000) // Informa ao compilador que estamos usando um Cristal de 20MHz.
                            // Esta informação é ESSENCIAL para que a função delay_ms()
                            // saiba calcular o tempo de espera corretamente.

// --- Definições (Não utilizadas neste código específico) ---
#define LED PIN_None // Esta linha veio de um template antigo, não é usada aqui.
#define DELAY 1000   // Esta linha também não é usada.

// --- Variáveis Globais ---
signed int8 flag = 1; // Esta é a "bandeira de direção". É a variável mais
                      // importante para a lógica do ping-pong.
                      // Se flag = 1, o contador SOMA (conta para cima).
                      // Se flag = -1, o contador SUBTRAI (conta para baixo).
                      // Começa em 1 para a contagem iniciar subindo.

// --- Mapas de Bits para os Displays de 7 Segmentos ---
// Estes arrays são a "tradução" de um número (ex: 5) para os
// bits que precisam ser enviados ao PORTB para acender os segmentos corretos.
// IMPORTANTE: Na McLab 1, o PORTB controla TUDO:
// 1. Quais segmentos acender (pinos RB0-RB6)
// 2. Qual display ligar (um pino de controle para o display da unidade
//    e outro para o da dezena, também no PORTB).
// Por isso, os valores de 'unidade' e 'dezena' são diferentes para o mesmo número.

byte const unidade[11] = {
    0b11111110, //0
    0b00111000, //1
    0b11011101, //2
    0b01111101, //3
    0b00111011, //4
    0b01110111, //5
    0b11110111, //6
    0b00111100, //7
    0b11111111, //8
    0b01111111  //9
};

byte const dezena[11] = {
    0b11101110, //0
    0b00101000, //1
    0b11001101, //2
    0b01101101, //3
    0b00101011, //4
    0b01100111, //5
    0b11100111, //6
    0b00101100, //7
    0b11101111, //8
    0b01101111  //9
};

// ==============================================================================
// --- FUNÇÃO PRINCIPAL (Onde tudo acontece) ---
// ==============================================================================
void main()
{
    // --- Declaração de Variáveis Locais ---
    unsigned int i = 0;     // Variável auxiliar. Vai guardar o dígito (0-9)
                            // que queremos mostrar (seja da dezena ou unidade).
    unsigned int tempo;     // Variável de controle para o loop 'for' do display.
    unsigned int cont = 25; // Esta é a variável principal! É o número que
                            // será mostrado no display. Começa em 25.

    // --- Loop Infinito ---
    // O microcontrolador ficará "preso" aqui para sempre,
    // executando o que estiver dentro destas chaves.
    while (true)
    {

        // --- ETAPA 1: MOSTRAR O NÚMERO NO DISPLAY (Por 1 segundo) ---
        // Este loop 'for' é o "motor" do display. Ele aplica a técnica de
        // MULTIPLEXAÇÃO. Ele pisca tão rápido entre os dois displays
        // que o olho humano pensa que os dois estão acesos ao mesmo tempo.
        //
        // Cálculo do tempo:
        // O loop roda 100 vezes.
        // Dentro dele, temos 2 delays de 5ms (5ms + 5ms = 10ms por ciclo).
        // Total = 100 ciclos * 10ms/ciclo = 1000ms = 1 SEGUNDO.
        //
        // Conclusão: Este loop 'for' inteiro serve para mostrar o valor
        // da variável 'cont' no display por exatamente 1 segundo.
        for (tempo = 0; tempo < 100; tempo++)
        {
            // --- Acende o display da DEZENA ---
            i = cont / 10;          // Pega a parte inteira da divisão (ex: 57 / 10 = 5)
            output_b(dezena[i]);    // Pega o padrão de bits do número 'i' (5) no array 'dezena'
                                    // e manda para o PORTB (ligando o display da dezena).
            delay_ms(5);            // Espera 5 milissegundos com ele aceso.

            // --- Acende o display da UNIDADE ---
            i = cont % 10;          // Pega o RESTO da divisão (ex: 57 % 10 = 7)
            output_b(unidade[i]);   // Pega o padrão de bits do número 'i' (7) no array 'unidade'
                                    // e manda para o PORTB (ligando o display da unidade).
            delay_ms(5);            // Espera 5 milissegundos com ele aceso.
        }
        // --- Fim do loop do display ---
        // Neste ponto, 1 segundo se passou.

        
        // --- ETAPA 2: LÓGICA DO CONTADOR "PING-PONG" ---
        // Esta parte só executa APÓS 1 segundo ter se passado.

        // Atualiza o valor do contador somando a 'flag'
        // Se flag=1, (cont = cont + 1) -> Sobe (ex: 25 -> 26)
        // Se flag=-1, (cont = cont + -1) -> Desce (ex: 60 -> 59)
        cont = cont + flag;

        // Verifica se o contador atingiu um dos limites
        if (cont >= 60 || cont <= 25)
        {
            // Se o contador chegou em 60 (enquanto subia)
            // OU
            // Se o contador chegou em 25 (enquanto descia)
            // ... então, nós invertemos a direção.

            // Multiplica a 'flag' por -1 para inverter seu sinal:
            // Se flag era 1 (subindo), vira -1 (começa a descer).
            // Se flag era -1 (descendo), vira 1 (começa a subir).
            flag = flag * (-1);
        }

    } // Fim do 'while(true)'. O programa volta ao início do 'while',
      // agora com o NOVO valor de 'cont', e o mostra no display
      // por mais 1 segundo.
}
