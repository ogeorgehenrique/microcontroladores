#include <16F877A.h>

// --- Configurações do Microcontrolador ---
#fuses NOWDT, NOBROWNOUT, NOLVP, HS // Fuses padrão para PICGenios
#use delay(crystal=20000000)      // Cristal de 20MHz

/*
 * =================================================================
 * MAPEAMENTO DE HARDWARE (Placa PICGenios)
 * =================================================================
 * Segmentos (a,b,c,d,e,f,g): PORTD (RD0 a RD6)
 *
 * Pinos de Controle dos Displays:
 * DISP 1 (Milhar):  PIN_A2
 * DISP 2 (Centena): PIN_A3
 * DISP 3 (Dezena):  PIN_A4  <-- VAMOS USAR ESTE
 * DISP 4 (Unidade): PIN_A5  <-- VAMOS USAR ESTE
 */
 
// Apelidos para os pinos que vamos usar
#define DISP_DEZENA   PIN_A4 // (DISP 3)
#define DISP_UNIDADE  PIN_A5 // (DISP 4)


// --- Mapa de Segmentos (Cátodo Comum) ---
// Define o padrão de bits em PORTD para formar cada número (0-9)
byte const display_map[10] = {
   0b00111111, // 0
   0b00000110, // 1
   0b01011011, // 2
   0b01001111, // 3
   0b01100110, // 4
   0b01101101, // 5
   0b01111101, // 6
   0b00000111, // 7
   0b01111111, // 8
   0b01101111  // 9
};

// --- Função Principal ---
void main() {

    // --- Configuração Inicial (Setup) ---
    set_tris_d(0x00); // Configura o PORTD (segmentos) como SAÍDA
    set_tris_a(0x00); // Configura o PORTA (controle dos displays) como SAÍDA

    // --- Variáveis do Programa ---
    int8 contador = 0;   // Nosso contador, vai de 0 a 10
    int8 dezena;         // Armazena o dígito da dezena
    int8 unidade;        // Armazena o dígito da unidade
    int i;               // Variável auxiliar para o loop de 1 segundo
    
    // --- Loop Infinito ---
    while (TRUE) {
        
        // 1. CALCULAR OS DÍGITOS
        // Pega o número do contador e o quebra em dois dígitos
        // Exemplo (contador = 10):
        dezena = contador / 10;   // 10 / 10 = 1
        unidade = contador % 10;  // 10 % 10 = 0 (resto)
        
        
        // 2. MOSTRAR NO DISPLAY (Multiplexação por 1 segundo)
        // Este loop 'for' é o nosso delay de 1 segundo.
        // Ele roda 100 vezes, e em cada ciclo ele pisca
        // a dezena e a unidade por 5ms cada (total 10ms).
        // 100 ciclos * 10ms = 1000ms = 1 segundo.
        for (i = 0; i < 100; i++) {
            
            // --- Acende o display da DEZENA (DISP 3) ---
            output_high(DISP_DEZENA);       // Liga o pino RA4
            output_d(display_map[dezena]);  // Envia o padrão do dígito (ex: '1')
            delay_ms(5);                    // Deixa aceso por 5ms
            output_low(DISP_DEZENA);        // Apaga (evita "ghosting")
            
            // --- Acende o display da UNIDADE (DISP 4) ---
            output_high(DISP_UNIDADE);      // Liga o pino RA5
            output_d(display_map[unidade]); // Envia o padrão do dígito (ex: '0')
            delay_ms(5);                    // Deixa aceso por 5ms
            output_low(DISP_UNIDADE);       // Apaga
        }
        
        // 3. ATUALIZAR O CONTADOR
        // Após 1 segundo mostrando o número, incrementa o contador
        contador++;
        
        // 4. VERIFICAR O LIMITE
        // Se o contador passou de 10, reinicia em 0
        if (contador > 10) {
            contador = 0;
        }
        
    } // Fim do while(TRUE), repete o ciclo com o novo número
}
