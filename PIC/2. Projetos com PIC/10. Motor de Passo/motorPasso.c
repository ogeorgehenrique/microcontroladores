/*
 * ==============================================================================
 * PROJETO: Controlador de Motor de Passo com Reversão por Interrupção
 * PLACA ALVO: PICGenios (ou similar)
 * MICROCONTROLADOR: PIC16F877A
 *
 * DESCRIÇÃO:
 * Este programa faz um motor de passo girar continuamente.
 * O sentido da rotação é controlado pela variável 'inc'.
 *
 * Uma interrupção externa no pino RB0 (Botão B1 na PICGenios)
 * é usada para inverter o valor de 'inc', mudando instantaneamente
 * o sentido de rotação do motor.
 * ==============================================================================
 */

#include <16F877A.h> // Biblioteca padrão do PIC16F877A
#device adc=8        // Define ADC para 8 bits (não usado neste código)

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

// --- Variáveis Globais de Controle do Motor ---

/*
 * Este array define a sequência de "passo completo" (two-phase)
 * para acionar as bobinas do motor. Os pinos RD0-RD3 são usados.
 * 3  = 0b00000011 (Bobinas A e B ligadas)
 * 6  = 0b00000110 (Bobinas B e C ligadas)
 * 12 = 0b00001100 (Bobinas C e D ligadas)
 * 9  = 0b00001001 (Bobinas D e A ligadas)
 */
unsigned int8 posicao[4] = {3, 6, 12, 9};

// Variável que armazena o passo atual na sequência (índice 0, 1, 2 ou 3)
signed int8 passo = 0;

// Variável de direção (incremento).
// Se inc = 1, o motor gira em um sentido (0, 1, 2, 3...)
// Se inc = -1, o motor gira no sentido oposto (3, 2, 1, 0...)
signed int8 inc = 1;

/*
 * ==============================================================================
 * FUNÇÃO DE INTERRUPÇÃO (ISR) EXTERNA (INT_EXT)
 * ==============================================================================
 * Esta diretiva '#INT_EXT' informa ao compilador:
 * "O código a seguir deve ser executado QUANDO o pino RB0
 * for pressionado (borda de descida)".
 */
#INT_EXT
void EXT_isr(void)
{
    // Limpa o "aviso" (flag) da interrupção para que ela possa ocorrer novamente
    clear_interrupt(INT_EXT);
    
    // INVERTE A DIREÇÃO DO MOTOR
    // Esta linha multiplica a variável de incremento por -1.
    // Se 'inc' era 1, vira -1.
    // Se 'inc' era -1, vira 1.
    inc = inc * (-1);
}

/*
 * ==============================================================================
 * FUNÇÃO PRINCIPAL (main)
 * ==============================================================================
 */
void main()
{
    // --- Configuração dos Periféricos ---
    // Desliga todos os módulos que não serão usados
    setup_adc_ports(NO_ANALOGS); // Pinos como digitais
    setup_adc(ADC_OFF);          // Desliga módulo ADC
    setup_psp(PSP_DISABLED);
    setup_spi(SPI_SS_DISABLED);
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
    setup_timer_1(T1_DISABLED);
    setup_timer_2(T2_DISABLED,0,1);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);

    // --- Configuração da Interrupção Externa (RB0) ---
    
    // Limpa qualquer "lixo" que possa estar na flag de interrupção
    clear_interrupt(INT_EXT);
    
    // Configura a interrupção para ser ativada na "borda de descida"
    // (H_TO_L = High to Low), que é o que acontece quando um
    // botão com pull-up é pressionado.
    ext_int_edge( H_TO_L );
    
    // Liga a "chave" específica para a interrupção externa (INT_EXT)
    enable_interrupts(INT_EXT);
    // Liga a "chave geral" que permite que QUALQUER interrupção aconteça
    enable_interrupts(GLOBAL);

    // --- Loop Infinito (Lógica do Motor) ---
    while(true)
    {
        // 1. Avança ou retrocede um passo na sequência,
        //    dependendo do valor de 'inc' (1 ou -1).
        passo = passo + inc;
        
        // 2. Lógica de "Wrap-around" (Dar a volta)
        // Se o passo passou do último (3), volta para o primeiro (0)
        if(passo > 3) passo = 0;
        // Se o passo foi menor que o primeiro (0), volta para o último (3)
        if(passo < 0) passo = 3;
        
        // 3. ACIONA O MOTOR
        // Envia o padrão de bits (ex: 0b0011) para o PORTD.
        // O PORTD está conectado ao driver (ex: ULN2003) que
        // energiza as bobinas corretas do motor.
        OUTPUT_D(posicao[passo]);
        
        // 4. CONTROLE DE VELOCIDADE
        // Espera 100ms antes de dar o próximo passo.
        // Menor o delay, mais rápido o motor gira.
        delay_ms(100);
        
    } // Fim do while(true), repete o ciclo
}
