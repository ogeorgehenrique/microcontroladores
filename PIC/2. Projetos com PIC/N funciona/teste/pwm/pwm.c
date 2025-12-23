/*
 * ==============================================================================
 * PROJETO: Controle de Motor DC (Soft-Start / Soft-Stop)
 * PLACA ALVO: PICGenios (PIC16F877A)
 *
 * DESCRIÇÃO:
 * - Controla a tensão média aplicada a um motor DC usando PWM.
 * - Botão START (B1): Inicia a "Rampa de Aceleração" (0% a 100%).
 * - Botão STOP (B0): Inicia a "Rampa de Desaceleração" (100% a 0%).
 * - O sinal PWM é gerado pelo hardware CCP1 (Pino RC2).




Admita que um motor corrente contínua é acionado via microcontrolador e drive. O perfil da tensão (sinal
PWM filtrado) aplicada ao motor está desenhado na figura 4. O processo é iniciado quando o operador
pressionar o botão start, e
finalizado com o botão stop.
O sinal de PWM pode ser
gerado via CCP ou via
software.


 * ==============================================================================
 */

#include <16F877A.h>
#device adc=10 // PWM de 10 bits (0 a 1023)

// --- Configuração dos Fuses ---
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=20000000)

// --- Definição dos Botões ---
#define BT_STOP  PIN_B0
#define BT_START PIN_B1

// --- Parâmetros do Perfil (Ajuste conforme a Figura 4) ---
#define PASSO_RAMPA 10   // Quanto o PWM aumenta/diminui por ciclo
#define DELAY_RAMPA 20   // Tempo entre cada incremento (define a suavidade)
                         // Quanto maior o delay, mais lenta é a partida.

void main() {
    // Variável para controlar o Duty Cycle (0 a 1023)
    unsigned int16 duty = 0;
    
    // Flag para saber se o motor está ligado ou desligado
    int1 motor_ligado = 0; 

    // --- Configuração dos Botões (Entrada) ---
    // Configura RB0 e RB1 como entrada
    // (Na PICGenios, geralmente usamos pull-ups internos ou resistores da placa)
    port_b_pullups(TRUE); 

    // --- Configuração do PWM (Hardware CCP1) ---
    // 1. Configura o Timer 2 (Base de tempo do PWM)
    setup_timer_2(T2_DIV_BY_16, 255, 1);
    
    // 2. Configura o módulo CCP1 para modo PWM
    setup_ccp1(CCP_PWM);
    
    // 3. Inicia com motor parado
    set_pwm1_duty(0);

    // --- Loop Infinito ---
    while(true) {

        // ---------------------------------------------------------
        // CENÁRIO 1: COMANDO DE PARTIDA (START - B1)
        // ---------------------------------------------------------
        if (!input(BT_START) && !motor_ligado) {
            // Entra no modo ligado
            motor_ligado = 1;
            
            // --- RAMPA DE ACELERAÇÃO (SOFT-START) ---
            // Aumenta o duty cycle de 0 até 1023 gradualmente
            for (duty = 0; duty < 1023; duty += PASSO_RAMPA) {
                set_pwm1_duty(duty);
                delay_ms(DELAY_RAMPA);
                
                // Segurança: Se apertar STOP durante a subida, cancela a subida
                if(!input(BT_STOP)) {
                   motor_ligado = 0;
                   break; 
                }
            }
            
            // Garante que chegou em 100% se não foi cancelado
            if(motor_ligado) {
                set_pwm1_duty(1023);
            }
        }

        // ---------------------------------------------------------
        // CENÁRIO 2: COMANDO DE PARADA (STOP - B0)
        // ---------------------------------------------------------
        if (!input(BT_STOP) && motor_ligado) {
            // Entra no modo desligado
            motor_ligado = 0;

            // --- RAMPA DE DESACELERAÇÃO (SOFT-STOP) ---
            // Diminui o duty cycle de 1023 até 0 gradualmente
            for (duty = 1023; duty > 0; duty -= PASSO_RAMPA) {
                set_pwm1_duty(duty);
                delay_ms(DELAY_RAMPA);
                
                // (Opcional) Se quiser permitir re-start durante a descida,
                // adicionaria lógica aqui.
            }
            
            // Garante que parou totalmente (0V)
            set_pwm1_duty(0);
        }
        
        // Pequeno delay para estabilidade do loop
        delay_ms(10);
    }
}
