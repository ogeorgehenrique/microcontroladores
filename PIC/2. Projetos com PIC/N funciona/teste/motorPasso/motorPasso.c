/*
 * ==============================================================================
 * 
 
 4. Desenvolva um programa para acionar o motor de passo a seguir. Para girar no sentido 1
(b1,b2,b3,b4,b1,b2..) e no sentido contrário devem ser acionadas as bobinas (b4,b3,b2,b1,b4,b3...). Utilize
um chave para selecionar o sentido (nível alto =sentido horário, nível baixo sentido anti-horário). O motor
deve girar com velocidade de 30rpm (rpm=rotações por minuto).
 
 PROJETO: Controle de Motor de Passo (30 RPM) com Reversão
 * PLACA ALVO: PICGenios
 * MICROCONTROLADOR: PIC16F877A
 *
 * DESCRIÇÃO:
 * - Controla um motor de passo de 1.2 graus/passo.
 * - Velocidade fixa de 30 RPM (Delay calculado: 6666us).
 * - Sentido controlado pelo pino RA2 (Chave).
 * - RA2 = 1 (5V) -> Sentido Horário
 * - RA2 = 0 (GND) -> Sentido Anti-Horário
 *
 * CONEXÕES (PICGenios):
 * - Motor de Passo: PORTD (RD0, RD1, RD2, RD3)
 * - Chave de Sentido: PIN_A2 (Ligar DIP Switch correspondente)
 * ==============================================================================
 */

#include <16F877A.h>

// --- Configuração dos Fuses ---
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=20000000) // Cristal de 20MHz

// --- Mapeamento de Hardware ---
// Bobinas do Motor ligadas no PORTD
// B1 -> RD0
// B2 -> RD1
// B3 -> RD2
// B4 -> RD3
#define CHAVE_SENTIDO PIN_A5 // Chave para controlar a direção

// --- Sequência de Acionamento (Single Phase / Wave Drive) ---
// Sequência solicitada: b1 -> b2 -> b3 -> b4
// Em binário (RD3 RD2 RD1 RD0):
// Passo 0: 0001 (0x01)
// Passo 1: 0010 (0x02)
// Passo 2: 0100 (0x04)
// Passo 3: 1000 (0x08)
const int8 passos[4] = {0x01, 0x02, 0x04, 0x08};

void main() {
    // Variável para controlar qual passo estamos executando (0 a 3)
    signed int8 indice = 0;

    // --- Configuração dos Portas ---
    set_tris_d(0x00); // PORTD todo como SAÍDA (Motor)
    set_tris_a(0xFF); // PORTA todo como ENTRADA (Chave)
    
    // Desliga comparadores analógicos para usar PORTA como digital
    setup_adc_ports(NO_ANALOGS);

    while(true) {
        
        // --- 1. Aciona o Motor ---
        // Envia o padrão de bits atual para o PORTD
        output_d(passos[indice]);

        // --- 2. Controle de Velocidade (30 RPM) ---
        // Calculado: 150 passos/segundo -> 6666us por passo
        delay_us(6666);

        // --- 3. Controle de Sentido ---
        if (input(CHAVE_SENTIDO)) {
            // --- Sentido Horário (b1 -> b2 -> b3 -> b4) ---
            indice++; // Avança na tabela
            
            // Verifica limite (Wrap-around)
            if (indice > 3) {
                indice = 0; // Volta para o primeiro passo
            }
        } 
        else {
            // --- Sentido Anti-Horário (b4 -> b3 -> b2 -> b1) ---
            indice--; // Recua na tabela
            
            // Verifica limite
            if (indice < 0) {
                indice = 3; // Vai para o último passo
            }
        }
    }
}
