/*
 * ==============================================================================
 * PROJETO: Semáforo de Pedestres Inteligente
 * PLACA ALVO: PICGenios (com PIC16F877A)
 *
 * DESCRIÇÃO:
 * Implementa um semáforo de carros e pedestres com as seguintes regras:
 * 1. Padrão: Carro Verde, Pedestre Vermelho.
 * 2. Ao apertar o botão:
 * a) Se o sinal abriu para carros há menos de 20s, o sistema espera
 * o tempo restante até completar 20s.
 * b) Inicia a sequência de fechamento para carros e abertura para pedestres.
 * 3. O pedestre tem um tempo total de 15s (10s fixo + 5s piscando)
 * para atravessar, com contagem regressiva no display.
 * ==============================================================================
 */

// --- 1. Inclusão de Bibliotecas e Configurações ---
#include <16F877A.h> // Biblioteca específica do PIC16F877A

// --- 2. Configuração dos FUSES ---
// Define as "configurações de hardware" do microcontrolador
#fuses XT, NOWDT, NOPROTECT, NOLVP
// XT: Cristal externo de 4MHz (XT = Cristal, 4-20MHz)
// NOWDT: Desabilita o Watchdog Timer (Cão de Guarda)
// NOPROTECT: Permite a leitura do código gravado (desprotegido)
// NOLVP: Desabilita a programação em baixa tensão (libera o pino RB3)

#use delay(clock=4000000) // Informa ao compilador que o clock é de 4MHz.
                         // Essencial para o delay_ms() funcionar corretamente.

// ---------- 3. Mapeamento de Pinos (Hardware) ----------
// Define "apelidos" para os pinos, facilitando a leitura do código.

// LEDs do semáforo de carros (conectados ao PORTB)
#define CARRO_VERMELHO  PIN_B1
#define CARRO_AMARELO   PIN_B2
#define CARRO_VERDE     PIN_B3

// LEDs do semáforo de pedestres (conectados ao PORTB)
#define PED_VERMELHO    PIN_B5
#define PED_VERDE       PIN_B6

// Botão de entrada (conectado ao PORTB)
#define BOTAO           PIN_B0  // O botão do pedestre é ativo em nível 0 (pressionado)

// Mapeamento dos Displays de 7 Segmentos
// Pinos de controle (seleção de qual display acende)
#define DISP_DEZENA     PIN_A4 // Pino que liga o display da DEZENA
#define DISP_UNIDADE    PIN_A5 // Pino que liga o display da UNIDADE

// Pinos de dados (segmentos a-g e "habilitador")
#define SEGMENT_ENABLE  PIN_D7 // Habilitador geral dos segmentos no PORTD

// ---------- 4. Tabela de Segmentos (Ânodo Comum) ----------
// Este é o "mapa" que traduz um número (0-9) em quais segmentos (a-g)
// devem ser ligados. O '1' significa "acende o segmento".
// A ordem é {a, b, c, d, e, f, g} - correspondendo a PIN_D0 até PIN_D6
const int8 segmentos[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
};

// ---------- 5. Funções Auxiliares (Modularização) ----------

/*
 * Função: display_off
 * Desliga completamente todos os displays e segmentos.
 * Útil para economizar energia e garantir que nada fique aceso
 * quando não deveria.
 */
void display_off() {
    // Coloca todos os 7 pinos de segmento (RD0-RD6) em 0
    for (int i = 0; i < 7; i++) {
       output_bit(PIN_D0 + i, 0);
    }
    // Desliga o habilitador geral (RD7)
    output_low(SEGMENT_ENABLE);
    // Desliga os pinos de controle dos displays (RA4, RA5)
    output_low(DISP_DEZENA);
    output_low(DISP_UNIDADE);
}

/*
 * Função: mostra_digito
 * É o núcleo da multiplexação. Acende UM dígito (dezena OU unidade)
 * com um 'valor' específico por um curto período de 5ms.
 */
void mostra_digito(int8 valor, int1 dezena) {
    // 1. Envia o padrão do 'valor' (ex: "5") para os pinos RD0-RD6
    for (int i = 0; i < 7; i++) {
       output_bit(PIN_D0 + i, segmentos[valor][i]);
    }
    // 2. Liga o habilitador geral (RD7) para energizar os segmentos
    output_high(SEGMENT_ENABLE);

    // 3. Liga o display específico (dezena OU unidade)
    if (dezena) { // Se o parâmetro 'dezena' for VERDADEIRO
       output_high(DISP_DEZENA); // Liga o display da dezena (RA4)
       delay_ms(5);              // Espera 5ms (o olho humano não percebe)
       output_low(DISP_DEZENA);  // Apaga o display da dezena
    } else { // Se for FALSO (é a unidade)
       output_high(DISP_UNIDADE); // Liga o display da unidade (RA5)
       delay_ms(5);               // Espera 5ms
       output_low(DISP_UNIDADE);  // Apaga o display da unidade
    }

    // 4. Desliga o habilitador geral (RD7) para evitar "ghosting"
    output_low(SEGMENT_ENABLE);
}

/*
 * Função: mostra_display
 * Exibe um número de dois dígitos ('valor') por um 'tempo_ms' determinado.
 * Ela fica chamando 'mostra_digito' repetidamente para criar a
 * ilusão de que os dois displays estão acesos ao mesmo tempo.
 */
void mostra_display(int8 valor, unsigned int16 tempo_ms) {
    int8 dez = valor / 10; // Pega o dígito da dezena (ex: 15 / 10 = 1)
    int8 uni = valor % 10; // Pega o dígito da unidade (ex: 15 % 10 = 5)
    unsigned int16 t = 0;  // Variável para controlar o tempo total

    // Loop de multiplexação. Roda até que o 'tempo_ms' tenha se passado.
    while (t < tempo_ms) {
       mostra_digito(dez, TRUE);  // Mostra a dezena por 5ms
       t += 5;                    // Adiciona 5ms ao tempo total
       mostra_digito(uni, FALSE); // Mostra a unidade por 5ms
       t += 5;                    // Adiciona 5ms ao tempo total
    }
    
    // Garante que o display apague ao final do tempo
    display_off(); 
}

/*
 * Função: botao_pressionado
 * Verifica se o botão foi pressionado. Inclui duas técnicas importantes:
 * 1. Debounce: Evita que ruídos elétricos sejam contados como múltiplos cliques.
 * 2. Wait-for-release: Espera o usuário soltar o botão antes de retornar
 * VERDADEIRO. Isso evita que o programa execute o ciclo várias vezes
 * se o usuário segurar o botão.
 */
int1 botao_pressionado() {
    if (!input(BOTAO)) { // 1. Verifica se o botão está pressionado (nível 0)
       delay_ms(20); // 2. Espera 20ms (filtro de "debounce")
       if (!input(BOTAO)) { // 3. Verifica NOVAMENTE se ainda está pressionado
          
          // 4. (Wait-for-release) Fica "preso" aqui enquanto o botão
          //    estiver sendo segurado.
          while (!input(BOTAO)) { delay_ms(10); } 
          
          // 5. Só retorna VERDADEIRO depois que o usuário soltar o botão.
          return TRUE;
       }
    }
    // Se não passou nas verificações, retorna FALSO.
    return FALSE;
}

/*
 * Função: estado_padrao
 * Configura os LEDs para o estado inicial e mais comum do semáforo:
 * Carro com sinal verde, Pedestre com sinal vermelho, Display apagado.
 */
void estado_padrao() {
    output_low(CARRO_VERMELHO);
    output_low(CARRO_AMARELO);
    output_high(CARRO_VERDE);

    output_high(PED_VERMELHO);
    output_low(PED_VERDE);

    display_off();
}

// ---------- 6. Programa Principal (main) ----------

void main() {
    // --- Variáveis de Controle de Tempo ---
    // Usamos 'unsigned long' (32 bits) para evitar que o contador "vire" (estouro).
    // O 'UL' no final força o número a ser tratado como unsigned long.
    unsigned long tempo = 20000UL;  // "Relógio" mestre do sistema, em ms.
                                    // Começa em 20000 para permitir o acionamento imediato.
    unsigned long ultimo = 0UL;     // "Timestamp" de quando o botão foi pressionado pela última vez.
    unsigned long decorrido, wait;  // Variáveis auxiliares para o cálculo de espera.
    unsigned int bloco;             // Auxiliar para o loop de espera
    int8 i;                         // Contador para os loops 'for' do display

    // --- Configuração Inicial (Setup) ---
    setup_adc(ADC_OFF);           // Desliga o conversor Analógico-Digital
    setup_adc_ports(NO_ANALOGS);  // Define todos os pinos como digitais
    
    // Configura a direção dos pinos
    set_tris_b(0b00000001); // PORTB: Apenas RB0 (BOTAO) é ENTRADA,
                            // o resto (LEDs) é SAÍDA.
    port_b_pullups(TRUE);   // Ativa os resistores de pull-up internos do PORTB
                            // (Garante que RB0 fique em nível 1 quando o botão está solto)
                            
    set_tris_d(0x00);       // PORTD (Segmentos) é todo SAÍDA
    set_tris_a(0x00);       // PORTA (Controle dos Displays) é todo SAÍDA

    // Inicia o semáforo no estado padrão
    estado_padrao();

    // --- Loop Infinito ---
    while (TRUE) {
       
       // 1. VERIFICA SE O BOTÃO FOI PRESSIONADO
       if (botao_pressionado()) {
          // Se foi, inicia a lógica de 20 segundos

          // 2. CALCULA O TEMPO DECORRIDO
          // Calcula quanto tempo (em ms) se passou desde a última vez
          // que o ciclo de pedestre foi iniciado.
          decorrido = (tempo >= ultimo) ? tempo - ultimo : 0UL;

          // 3. VERIFICA A REGRA DOS 20 SEGUNDOS
          if (decorrido < 20000UL) { // Se menos de 20s se passaram
             // Calcula quanto tempo FALTA para completar os 20s
             wait = 20000UL - decorrido;
             
             // Fica "preso" aqui, esperando o tempo que falta.
             // O 'bloco' é para o delay_ms não travar se o 'wait' for muito grande.
             while (wait) {
                bloco = (wait >= 1000UL) ? 1000 : (unsigned int)wait;
                delay_ms(bloco); // Espera no máximo 1 segundo de cada vez
                tempo += bloco;  // Atualiza o relógio mestre
                wait -= bloco;   // Diminui o tempo restante
             }
          }

          // 4. ATUALIZA O "TIMESTAMP"
          // Marca o 'agora' como o "último acionamento"
          ultimo = tempo;

          // ==========================================================
          // --- INÍCIO DA SEQUÊNCIA DE TRAVESSIA DE PEDESTRES ---
          // ==========================================================

          // 5) Carro Amarelo (3 segundos)
          output_low(CARRO_VERDE);
          output_high(CARRO_AMARELO);
          delay_ms(3000);
          tempo += 3000; // Atualiza o relógio mestre
          output_low(CARRO_AMARELO);

          // 6) Carro Vermelho (2 segundos de segurança)
          output_high(CARRO_VERMELHO);
          delay_ms(2000);
          tempo += 2000;

          // 7) Pedestre Verde (10 segundos) - Contagem de 15 a 6
          output_low(PED_VERMELHO);
          output_high(PED_VERDE);
          // Loop de contagem regressiva
          for (i = 15; i >= 6; i--) {
             // Mostra o número 'i' no display por 1 segundo
             mostra_display(i, 1000); 
             tempo += 1000; // Atualiza o relógio mestre
          }

          // 8) Pedestre Verde PISCANDO (6 segundos) - Contagem de 5 a 0
          // O loop `for (i = 5; i != 255; i--)` é uma forma de fazer
          // `i >= 0`. Quando 'i' é 0 e subtrai 1, ele "vira" 255 (estouro)
          // e o loop para.
          for (i = 5; i != 255; i--) {
             // Mostra o número por 0.5 segundos
             mostra_display(i, 500);
             tempo += 500;
             // Apaga o display (display_off é chamado dentro de mostra_display)
             delay_ms(500); // Fica apagado por 0.5 segundos (criando o "pisca")
             tempo += 500;
          }

          // 9) Fim da travessia: Pedestre volta ao Vermelho
          output_low(PED_VERDE);
          output_high(PED_VERMELHO);
          display_off(); // Garante que o display apagou

          // 10) Tempo de Segurança (1 segundo)
          // (Carro e Pedestre ficam no vermelho)
          delay_ms(1000);
          tempo += 1000;
          
          // 11) Retorna ao estado padrão (Carro Verde)
          estado_padrao(); 
          
          // A linha abaixo é redundante, pois estado_padrao() já faz isso,
          // mas serve como reforço visual.
          output_low(CARRO_VERMELHO);
          output_high(CARRO_VERDE);
       }
       
       // 12. ATUALIZAÇÃO DO RELÓGIO MESTRE
       // Esta é a "batida do coração" do sistema.
       // O programa espera 50ms e atualiza o relógio 'tempo'.
       // Isso garante que o 'tempo' continue contando mesmo quando
       // o botão não está sendo pressionado.
       delay_ms(50);
       tempo += 50;
    } // Fim do while(TRUE)
} // Fim do main()
