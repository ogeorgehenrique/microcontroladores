#include "kbd_ext_board2.c"
#include "mod_lcd.c"



/*
 * ==============================================================================
 * PROJETO: Leitor de Teclado Matricial com Display LCD
 * PLACA ALVO: PICGenios
 * MICROCONTROLADOR: PIC16F877A
 *
 * DESCRIÇÃO:
 * Este programa lê as teclas pressionadas em um teclado matricial (keypad)
 * e exibe o caractere correspondente no display LCD.
 *
 * ARQUIVOS EXTERNOS NECESSÁRIOS:
 * 1. mod_lcd.c: Driver para controlar o display LCD.
 * 2. kbd_ext_board2.c: Driver para "escanear" o teclado matricial.
 * ==============================================================================
 */

#include <16F877A.h> // Biblioteca padrão do PIC16F877A

// Define a resolução do AD para 8 bits (0-255).
// NOTA: Este módulo (ADC) é desligado logo abaixo, então esta linha
// não tem efeito prático neste código.
#device adc=8

// --- Configuração dos Fuses ---
#FUSES HS         // Oscilador High Speed (para o cristal de 20MHz)
#FUSES NOPUT      // Desabilita o Power Up Timer
#FUSES NOPROTECT  // Código não protegido contra leitura
#FUSES NODEBUG    // Modo Debug desligado
#FUSES NOBROWNOUT // Desabilita reset por queda de tensão
#FUSES NOLVP      // Desabilita baixa tensão (libera pino RB3)
#FUSES NOCPD      // Proteção de dados da EEPROM desligada
#FUSES NOWRT      // Memória de programa não protegida contra escrita

// Informa ao compilador que o cristal é de 20MHz
#use delay(clock=20000000)

// --- Mapeamento dos Pinos do LCD (Padrão PICGenios) ---
#ifndef lcd_enable
 #define lcd_enable pin_E1 // pino enable (E) do LCD -> conectado ao RE1
 #define lcd_rs pin_E2     // pino rs (Register Select) do LCD -> conectado ao RE2
 //#define lcd_rw pin_e2   // pino rw (Read/Write) do LCD (comentado)
 #define lcd_d4 pin_d4     // pino de dados d4 do LCD -> conectado ao RD4
 #define lcd_d5 pin_d5     // pino de dados d5 do LCD -> conectado ao RD5
 #define lcd_d6 pin_d6     // pino de dados d6 do LCD -> conectado ao RD6
 #define lcd_d7 pin_d7     // pino de dados d7 do LCD -> conectado ao RD7
#endif

// --- Inclusão dos Drivers Externos ---
// (Estes arquivos .c devem estar na pasta do seu projeto)

// Inclui o driver do LCD. Fornece funções como lcd_ini() e lcd_escreve()
#include "mod_lcd.c"
// Inclui o driver do Teclado. Fornece a função tc_tecla()
#include "kbd_ext_board2.c"

/*
 * ==============================================================================
 * FUNÇÃO PRINCIPAL
 * ==============================================================================
 */
void main()
{
    // Variável para armazenar a tecla que foi pressionada.
    // 'unsigned char' armazena um valor de 0 a 255.
    unsigned char tmp;

    // --- Configuração Inicial (Setup) ---
    // Desliga todos os periféricos que não serão usados
    // para economizar energia e evitar conflitos de pinos.
    setup_adc_ports(NO_ANALOGS); // Define todos os pinos como digitais
    setup_adc(ADC_OFF);          // Desliga o módulo Conversor A/D
    setup_psp(PSP_DISABLED);     // Desliga a Porta Paralela
    setup_spi(SPI_SS_DISABLED);  // Desliga o barramento SPI
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1); // Configura Timer 0
    setup_timer_1(T1_DISABLED);  // Desliga Timer 1
    setup_timer_2(T2_DISABLED,0,1); // Desliga Timer 2
    setup_comparator(NC_NC_NC_NC); // Desliga os comparadores
    setup_vref(FALSE);             // Desliga a referência de tensão interna

    // --- Inicialização do LCD ---
    lcd_ini(); // Chama a função de inicialização do driver do LCD
    delay_ms(10); // Pequena pausa para o LCD estabilizar

    // --- Tela de Boas-Vindas ---
    // \f = Comando "Form Feed", limpa a tela do LCD
    printf (lcd_escreve,"\fIFMT ... 2022");
    delay_ms(1000); // Mostra a mensagem por 1 segundo

    printf (lcd_escreve,"\f TECLADO "); // Limpa e escreve "TECLADO"
    delay_ms(2000); // Mostra por 2 segundos

    // --- Loop Infinito (Lógica Principal) ---
    while (true)
    {

        // --- 1. Leitura do Teclado ---
        // Chama a função 'tc_tecla' (do driver kbd_ext_board2.c)
        // A função fica "escutando" o teclado por até 1500ms (1.5 segundos).
        // - Se uma tecla for pressionada, 'tmp' recebe o código ASCII dela (ex: '5').
        // - Se nenhuma tecla for pressionada (timeout), 'tmp' recebe o valor 255.
        //-------------------------------------------
        tmp = tc_tecla(1500); // ms
        //-------------------------------------------
        
        // --- 2. Verificação do Resultado ---
        
        // Se 'tmp' for DIFERENTE de 255, significa que uma tecla FOI pressionada.
        if(tmp != 255)
        {
            // Limpa a tela (\f) e mostra qual tecla foi pressionada.
            // O '%c' formata o valor de 'tmp' como um caractere.
            // (Provavelmente o autor quis dizer "Button")
            printf (lcd_escreve,"\f Botton %c",tmp);
        }
        else // Se 'tmp' for IGUAL a 255 (timeout, nenhuma tecla pressionada)
        { 
            // Limpa a tela e mostra a mensagem "TECLADO L"
            // (provavelmente "Teclado Livre" ou "Lendo")
            printf (lcd_escreve,"\f TECLADO L");
        }

    } // Fim do laço infinito, volta a ler o teclado
}
