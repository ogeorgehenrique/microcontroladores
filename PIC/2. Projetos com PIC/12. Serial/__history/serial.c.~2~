/*
 * ==============================================================================
 * PROJETO: Transmissor Serial (RS232)
 * PLACA ALVO: PICGenios
 * MICROCONTROLADOR: PIC16F877A
 *
 * DESCRIÇÃO:
 * Este programa configura a comunicação serial (RS232) do PIC
 * e envia continuamente o caractere 'A', seguido de um espaço
 * e uma quebra de linha, a cada 100 milissegundos.
 *
 * É um exemplo clássico de "Hello, World!" para a porta serial.
 *
 * NOTA: O código de configuração do ADC (Conversor A/D) é iniciado,
 * mas nunca é usado no loop principal (é "código morto").
 * ==============================================================================
 */

#include <16F877A.h> // Biblioteca padrão do PIC16F877A
#device adc=8        // Define o ADC para 8 bits (não usado neste código)

// --- Configuração dos Fuses ---
#FUSES NOWDT      // Desabilita o Watch Dog Timer
#FUSES HS         // Oscilador High Speed (para o cristal de 20MHz)
#FUSES NOPUT      // Desabilita o Power Up Timer
#FUSES NOPROTECT  // Código não protegido contra leitura
#FUSES NODEBUG    // Modo Debug desligado
#FUSES BROWNOUT   // ATIVA o reset por queda de tensão
#FUSES NOLVP      // Desabilita baixa tensão (libera pino RB3)
#FUSES NOCPD      // Proteção de dados da EEPROM desligada
#FUSES NOWRT      // Memória de programa não protegida contra escrita

// Informa ao compilador que o cristal é de 20MHz
#use delay(clock=20000000)

// --- CONFIGURAÇÃO DA PORTA SERIAL (RS232 / USART) ---
// Esta é a linha mais importante do programa.
// Ela habilita o hardware de comunicação serial do PIC.
#use rs232(
    baud=9600,       // Define a velocidade da comunicação: 9600 bits por segundo.
    parity=N,        // Sem paridade (um tipo de verificação de erro).
    xmit=PIN_C6,     // Define o pino de TRANSMISSÃO (TX) como RC6.
    rcv=PIN_C7,      // Define o pino de RECEBIMENTO (RX) como RC7.
    bits=8           // Define o pacote de dados como 8 bits.
)
// Na PICGenios, os pinos RC6 e RC7 são conectados ao chip MAX232
// para a comunicação com o computador via cabo serial.

/*
 * ==============================================================================
 * FUNÇÃO PRINCIPAL (main)
 * ==============================================================================
 */
void main()
{
    // --- "Código Morto" (Não utilizado) ---
    // Esta variável é declarada, mas nunca usada no programa.
    unsigned int valor = 8;
    
    // Este bloco inteiro configura o Conversor A/D (ADC).
    // No entanto, a função 'read_adc()' nunca é chamada no loop 'while(true)'.
    // Portanto, esta configuração não tem efeito prático no resultado final.
    setup_adc_ports(AN0_AN1_AN3);
    setup_adc(ADC_CLOCK_DIV_16);
    
    // --- Desabilita outros periféricos não usados ---
    setup_psp(PSP_DISABLED);
    setup_spi(SPI_SS_DISABLED);
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
    setup_timer_1(T1_DISABLED);
    setup_timer_2(T2_DISABLED,0,1);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);
    // --- Fim do "Código Morto" ---

    // --- Loop Infinito (Transmissão Serial) ---
    while(true)
    {
        // 1. Envia o caractere 'A' (código ASCII 65) pela porta serial (pino TX/RC6).
        putc('A');
        
        // Esta linha (comentada) faria exatamente a mesma coisa que a linha de cima.
        // putc(65);
        
        // 2. Envia o caractere 'espaço' (código ASCII 32).
        putc(' ');
        
        // 3. Envia o caractere 'nova linha' (ou "line feed", código ASCII 10).
        //    Isso faz o cursor no terminal do computador pular para a linha de baixo.
        putc('\n');
        
        // 4. Espera 100 milissegundos antes de repetir o loop.
        //    Resultado: O 'A' é enviado 10 vezes por segundo.
        delay_ms(100);
        
    } // Fim do while(true), o ciclo recomeça
}
