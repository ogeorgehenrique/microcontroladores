/*
 * ==============================================================================
 * PROJETO: Efeito de "Fade" ou "Respiração" (Breathing) com LED
 * PLACA ALVO: PICGenios (ou qualquer placa com um LED no pino RD2)
 * MICROCONTROLADOR: PIC16F877A
 *
 * DESCRIÇÃO:
 * Este código faz um LED aumentar e diminuir seu brilho suavemente,
 * criando um efeito de "respiração".
 *
 * 1. (Fade In): O brilho do LED aumenta de 0% para 100% em 10 segundos.
 * 2. (Fade Out): O brilho do LED diminui de 100% para 0% em 10 segundos.
 * 3. O ciclo recomeça indefinidamente.
 *
 * TÉCNICA UTILIZADA:
 * A simulação de brilho é feita usando PWM (Modulação por Largura de Pulso)
 * por software. O código pisca o LED 1000 vezes por segundo (frequência de 1kHz).
 * O "brilho" é controlado pela porcentagem de tempo que o LED
 * fica ACESO (Ton) vs. APAGADO (Toff) dentro de cada ciclo de 1ms (1000us).
 *
 * (Ton + Toff = 1000us)
 * ==============================================================================
 */

#include <16F877A.h> // Biblioteca padrão do PIC16F877A

#device ADC=10 // Define a resolução do AD para 10 bits (não usado neste código)

// --- Configuração dos Fuses ---
#FUSES NOWDT      // Desabilita o Watch Dog Timer
#FUSES NOBROWNOUT // Desabilita o reset por queda de tensão
#FUSES NOLVP      // Desabilita a programação em baixa tensão (libera pino RB3)

// Informa ao compilador que o cristal é de 20MHz (essencial para delays)
#use delay(crystal=20000000)

// --- Mapeamento de Pinos ---
#define LED PIN_D2 // Define o "apelido" LED para o pino RD2

// --- Variáveis Globais ---
int16 Ton=0;  // Armazena o tempo que o LED ficará LIGADO (em microssegundos)
int16 Toff=0; // Armazena o tempo que o LED ficará DESLIGADO (em microssegundos)

/*
 * ==============================================================================
 * FUNÇÃO PRINCIPAL
 * ==============================================================================
 */
void main()
{
    // Loop infinito que garante que o efeito se repita para sempre
    while(true)
    {
        /*
         * =================================================================
         * LOOP 1: FADE IN (Aumentando o brilho) - Duração: 10 segundos
         * =================================================================
         * Este loop 'for' roda 10.000 vezes.
         * Cada ciclo do loop dura 1000us (1ms) (Ton + Toff).
         * Duração total: 10.000 ciclos * 1ms/ciclo = 10.000 ms = 10 segundos.
         */
        for(int16 i=0; i<10000; i++)
        {
            // Acende o LED
            output_high(LED);
            
            // Calcula o tempo que o LED ficará ACESO (Ton)
            // O 'i' vai de 0 a 10.000.
            // 'Ton' vai de (0/10)=0us até (10000/10)=1000us.
            Ton = i/10;
            
            // Espera o tempo 'Ton' com o LED aceso
            delay_us(Ton);
            
            // Apaga o LED
            output_low(LED);
            
            // Calcula o tempo restante que o LED ficará APAGADO (Toff)
            // O período total é 1000us.
            // Se Ton=300us, Toff=700us. Se Ton=900us, Toff=100us.
            Toff = 1000 - Ton;
            
            // Espera o tempo 'Toff' com o LED apagado
            delay_us(Toff);
        }
        
        /*
         * =================================================================
         * LOOP 2: FADE OUT (Diminuindo o brilho) - Duração: 10 segundos
         * =================================================================
         * Este loop 'for' roda 10.000 vezes (de 10.000 para 0).
         * Cada ciclo também dura 1000us (1ms).
         * Duração total: 10.000 ciclos * 1ms/ciclo = 10.000 ms = 10 segundos.
         */
        for(int16 j=10000; j>0; j--)
        {
            // Acende o LED
            output_high(LED);
            
            // Calcula o tempo que o LED ficará ACESO (Ton)
            // O 'j' vai de 10.000 para 0.
            // 'Ton' vai de (10000/10)=1000us até (0/10)=0us.
            Ton = j/10;
            
            // Espera o tempo 'Ton' com o LED aceso
            delay_us(Ton);
            
            // Apaga o LED
            output_low(LED);
            
            // Calcula o tempo restante que o LED ficará APAGADO (Toff)
            Toff = 1000 - Ton;
            
            // Espera o tempo 'Toff' com o LED apagado
            delay_us(Toff);
        }
        
    } // Fim do while(true), o ciclo recomeça
}
