/*
A frequência (f) é quantas vezes o LED pisca por segundo. O período total (T) é 1000ms/f.
Como queremos que ele acenda e apague (50% ligado, 50% desligado),
o tempo de troca (Toggle) é a metade do período.
LED 1 (2 Hz): 1000/2 = 500ms. Troca a cada 250ms.
LED 2 (3 Hz): 1000/3 = 333ms. Troca a cada ~166ms.
LED 3 (5 Hz): 1000/5 = 200ms. Troca a cada 100ms.
LED 4 (8 Hz): 1000/8 = 125ms. Troca a cada ~62ms.
LED 5 (13 Hz): 1000/13 = 76ms. Troca a cada ~38ms.
LED 6 (21 Hz): 1000/21 = 47ms. Troca a cada ~23ms.

*/





#include <16F877A.h>

// --- Configurações dos Fuses (Padrão PICGenios) ---
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=20000000) // Cristal de 20MHz

// --- Definição dos LEDs no PORTD (Padrão PICGenios) ---
#define LED1 PIN_D0
#define LED2 PIN_D1
#define LED3 PIN_D2
#define LED4 PIN_D3
#define LED5 PIN_D4
#define LED6 PIN_D5

// --- Definição dos Tempos de Troca (Calculados acima) ---
#define TEMPO_LED1 250 // 2 Hz
#define TEMPO_LED2 166 // 3 Hz
#define TEMPO_LED3 100 // 5 Hz
#define TEMPO_LED4 62  // 8 Hz
#define TEMPO_LED5 38  // 13 Hz
#define TEMPO_LED6 23  // 21 Hz

void main() {
    // Configura o PORTD como saída (onde estão os LEDs)
    set_tris_d(0x00);
    output_d(0x00); // Começa com tudo apagado

    // --- Variáveis Contadoras ---
    // Elas vão contar quantos milissegundos se passaram para cada LED
    unsigned int16 cont1 = 0;
    unsigned int16 cont2 = 0;
    unsigned int16 cont3 = 0;
    unsigned int16 cont4 = 0;
    unsigned int16 cont5 = 0;
    unsigned int16 cont6 = 0;

    while(true) {
        
        // --- BASE DE TEMPO (O Coração do Código) ---
        // Atrasamos apenas 1ms por ciclo. Isso é rápido o suficiente
        // para parecer que tudo está acontecendo ao mesmo tempo.
        delay_ms(1);

        // Incrementa todos os contadores
        cont1++;
        cont2++;
        cont3++;
        cont4++;
        cont5++;
        cont6++;

        // --- Lógica do LED 1 (2 Hz) ---
        if (cont1 >= TEMPO_LED1) {
            output_toggle(LED1); // Inverte o estado do LED
            cont1 = 0;           // Reinicia o contador dele
        }

        // --- Lógica do LED 2 (3 Hz) ---
        if (cont2 >= TEMPO_LED2) {
            output_toggle(LED2);
            cont2 = 0;
        }

        // --- Lógica do LED 3 (5 Hz) ---
        if (cont3 >= TEMPO_LED3) {
            output_toggle(LED3);
            cont3 = 0;
        }

        // --- Lógica do LED 4 (8 Hz) ---
        if (cont4 >= TEMPO_LED4) {
            output_toggle(LED4);
            cont4 = 0;
        }

        // --- Lógica do LED 5 (13 Hz) ---
        if (cont5 >= TEMPO_LED5) {
            output_toggle(LED5);
            cont5 = 0;
        }

        // --- Lógica do LED 6 (21 Hz) ---
        if (cont6 >= TEMPO_LED6) {
            output_toggle(LED6);
            cont6 = 0;
        }
    }
}
