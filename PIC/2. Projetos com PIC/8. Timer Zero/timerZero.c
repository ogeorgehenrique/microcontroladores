/*
 * ==============================================================================
 * PROJETO: Demonstração de Interrupção com Timer 0 (RTCC)
 * PLACA ALVO: PICGenios
 * MICROCONTROLADOR: PIC16F877A
 *
 * DESCRIÇÃO:
 * Este programa demonstra como usar as interrupções do Timer 0 (RTCC).
 * O programa principal (main) não faz nada; ele apenas configura o timer
 * e entra em um loop vazio.
 *
 * O Timer 0 é configurado para "estourar" (overflow) a cada 13.1ms.
 * A cada estouro, ele gera uma INTERRUPÇÃO, que pausa o loop 'main'
 * momentaneamente e executa a função 'RTCC_isr'.
 *
 * FUNCIONAMENTO ATUAL:
 * A função 'RTCC_isr' simplesmente inverte o estado do pino RC2
 * (LED RL1 na PICGenios) a cada 13.1ms. Isso cria um pisca-pisca
 * de alta frequência (~38 Hz), fazendo o LED parecer aceso
 * com metade do brilho (efeito "dimmer").
 *
 * FUNCIONAMENTO COMENTADO:
 * O código comentado dentro da ISR implementa um contador para
 * criar um pisca-pisca de 1 segundo no pino RD0 (LED RL4).
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
#FUSES NOBROWNOUT // Desabilita reset por queda de tensão
#FUSES NOLVP      // Desabilita baixa tensão (libera pino RB3)
#FUSES NOCPD      // Proteção de dados da EEPROM desligada
#FUSES NOWRT      // Memória de programa não protegida contra escrita
#FUSES RESERVED   // Bits de fuse reservados

// Informa ao compilador que o cristal é de 20MHz
#use delay(clock=20000000)

// --- Variável Global ---
// Esta variável seria usada pela lógica comentada para contar
// o número de interrupções.
unsigned int8 contador=0;

/*
 * ==============================================================================
 * FUNÇÃO DE INTERRUPÇÃO (ISR) DO TIMER 0 (RTCC)
 * ==============================================================================
 * Esta diretiva '#int_RTCC' informa ao compilador:
 * "O código a seguir deve ser executado QUANDO o Timer 0 estourar".
 */
#int_RTCC
void RTCC_isr(void)
{
    // --- Código Ativo ---
    // Inverte o estado do pino RC2 (LED RL1 na PICGenios).
    // Como a interrupção ocorre a cada 13.1ms, o LED pisca
    // a uma frequência de ~38Hz (1 / (2 * 13.1ms)).
    // O olho humano não vê a piscada, apenas o LED com brilho reduzido.
    output_toggle(PIN_C2);

    /*
    // --- Código Comentado (Intenção Original) ---
    // Esta era a lógica provável para criar um pisca-pisca de 1 segundo.

    // 1. Incrementa o contador a cada interrupção (a cada 13.1ms)
    contador = contador + 1;
    
    // 2. Verifica se o contador atingiu 76
    // Cálculo: 76 * 13.1ms = 995.6ms (aproximadamente 1 segundo)
    if(contador >= 76)
    {
        // 3. Pisca o LED no pino RD0 (LED RL4 na PICGenios)
        output_toggle(PIN_D0);
        
        // 4. Zera o contador para começar a contar o próximo segundo
        contador = 0;
    }
    */
} // Fim da rotina de interrupção


/*
 * ==============================================================================
 * FUNÇÃO PRINCIPAL (main)
 * ==============================================================================
 */
void main()
{
    // --- Configuração dos Periféricos ---
    // Desliga todos os periféricos que não serão usados
    setup_adc_ports(NO_ANALOGS); // Todos os pinos como digitais
    setup_adc(ADC_OFF);          // Desliga o módulo ADC
    setup_psp(PSP_DISABLED);
    setup_spi(SPI_SS_DISABLED);

    // --- Configuração do Timer 0 (RTCC) ---
    // Esta é a linha mais importante do setup.
    // RTCC_INTERNAL: Usa o clock interno do PIC (Fosc/4)
    // RTCC_DIV_256:  Usa o "prescaler" (divisor de frequência) máximo, 256.
    // Isso torna o timer 256x mais lento.
    // O resultado é um "estouro" (overflow) a cada 13.1 milissegundos.
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256); // Overflow 13,1 ms

    // Desliga os outros timers
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
    setup_timer_2(T2_DISABLED,0,1);
    
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);

    // --- Habilitação das Interrupções ---
    // Liga a "chave" específica para a interrupção do Timer 0 (RTCC)
    enable_interrupts(INT_RTCC);
    // Liga a "chave geral" que permite que QUALQUER interrupção aconteça
    enable_interrupts(GLOBAL);

    // --- Loop Infinito Vazio ---
    // O programa principal fica "preso" aqui e não faz nada.
    // A CPU fica ociosa, esperando.
    // Todo o "trabalho" (piscar o LED) é feito em "segundo plano"
    // pela rotina de interrupção 'RTCC_isr', que é chamada
    // automaticamente pelo hardware do Timer 0.
    while(true)
    {
        // Aguardando interrupções...
    }
}
