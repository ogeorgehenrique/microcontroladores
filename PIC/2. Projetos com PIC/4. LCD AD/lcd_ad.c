#include <16F877A.h> // Biblioteca principal do microcontrolador PIC16F877A

// Define a resolução do Conversor AD para 10 bits.
// Isso significa que a leitura analógica (0-5V) será convertida
// para um número digital entre 0 e 1023.
#device adc=10 

// --- Configuração dos Fuses (Configurações de "Hardware" do PIC) ---
#FUSES NOWDT      // Desabilita o "Cão de Guarda" (Watch Dog Timer)
#FUSES HS         // Define o oscilador para "High Speed" (para cristais > 4MHz, como o de 20MHz)
#FUSES NOPUT      // Desabilita o "Power Up Timer"
#FUSES NOPROTECT  // O código não é protegido contra leitura
#FUSES NODEBUG    // Modo de depuração desligado
#FUSES NOBROWNOUT // Sem reset por queda de tensão
#FUSES NOLVP      // Desabilita a programação em baixa tensão (libera o pino RB3)
#FUSES NOCPD      // Proteção de dados da EEPROM desligada
#FUSES NOWRT      // A memória de programa não é protegida contra escrita
#FUSES RESERVED   // Define bits de fuse reservados (não usados)

// Informa ao compilador que o cristal externo é de 20MHz.
// Essencial para que 'delay_ms()' funcione no tempo correto.
#use delay(clock=20000000)

// --- Mapeamento dos Pinos do LCD ---
// Estas linhas definem quais pinos do PIC controlam o LCD.
// Este mapeamento é o padrão da placa PICGenios.
#ifndef lcd_enable
#define lcd_enable pin_E1 // pino enable (E) do LCD -> conectado ao RE1
#define lcd_rs pin_E2     // pino rs (Register Select) do LCD -> conectado ao RE2
//#define lcd_rw pin_A4   // pino rw (Read/Write) do LCD (comentado, prov. aterrado na placa)
#define lcd_d4 pin_d4     // pino de dados d4 do LCD -> conectado ao RD4
#define lcd_d5 pin_d5     // pino de dados d5 do LCD -> conectado ao RD5
#define lcd_d6 pin_d6     // pino de dados d6 do LCD -> conectado ao RD6
#define lcd_d7 pin_d7     // pino de dados d7 do LCD -> conectado ao RD7
#endif

// --- Inclusão do Driver do LCD ---
// ATENÇÃO: Esta linha é um "caminho absoluto".
// O compilador tentará encontrar o arquivo 'mod_lcd.c' na pasta
// 'C:\Alberto\...' no seu computador.
// Isso VAI CAUSAR UM ERRO DE COMPILAÇÃO "File not found"
// se você não tiver esse arquivo exatamente nesse caminho.
#include "mod_lcd.c"

// --- Função Principal ---
void main()
{
    // Variáveis para armazenar os valores lidos pelo AD (0-1023)
    unsigned int16 valor1=0, valor2=0;
    
    // Variáveis para converter o valor em tensão (não estão sendo usadas)
    float tensao1=0, tensao2=0;
    
    // --- Configuração dos Periféricos ---
    
    // Configura quais pinos serão usados como entradas analógicas (AD)
    // AN0 (RA0) -> Sensor de Temperatura LM35 na PICGenios
    // AN1 (RA1) -> Sensor de Luz LDR na PICGenios
    // AN3 (RA3) -> Potenciômetro (Trimpot) na PICGenios
    setup_adc_ports(AN0_AN1_AN3);
    
    // Define o clock (velocidade) do conversor AD
    setup_adc(ADC_CLOCK_DIV_16);
    
    // Desabilita periféricos que não serão usados neste projeto
    setup_psp(PSP_DISABLED);
    setup_spi(SPI_SS_DISABLED);
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
    setup_timer_1(T1_DISABLED);
    setup_timer_2(T2_DISABLED,0,1);
    setup_comparator(NC_NC_NC_NC); // Desliga os comparadores analógicos
    setup_vref(FALSE); // Desliga a referência de tensão interna

    // --- Inicialização do LCD ---
    lcd_ini();     // Inicializa o display (função que está no 'mod_lcd.c')
    delay_ms(50);  // Pequena pausa para o LCD estabilizar
    
    // --- Tela de Boas-Vindas ---
    lcd_escreve ('\f'); // Envia o comando "Limpar Tela" (Form Feed)
    printf (lcd_escreve," IFMT 2025 \r\n"); // Escreve " IFMT 2025 " na linha 1
    printf (lcd_escreve," LCD e AD ");      // Escreve " LCD e AD " na linha 2
    delay_ms (2000); // Mostra a mensagem por 2 segundos

    // --- Loop Infinito (Leitura dos Sensores) ---
    while(true){
    
        // --- Leitura do Canal 1 (Temperatura) ---
        set_adc_channel(0);  // 1. Seleciona o Canal 0 (AN0 / RA0)
        delay_us(50);        // 2. Espera 50 microssegundos para o canal estabilizar
        valor1 = read_adc(); // 3. Faz a leitura e armazena em 'valor1'
        delay_us(50);
        delay_ms(150);       // Pausa (para não sobrecarregar)
        
        // 4. Mostra o valor1 no LCD
        // \f = Limpa a tela
        // %Lu = Formato para 'long unsigned int' (int16)
        // \n\r = Pula para a próxima linha
        printf(lcd_escreve,"\fA/D value1 = %Lu\n\r", valor1);
        delay_us(50);
        
        // --- Leitura do Canal 2 (Luz LDR) ---
        set_adc_channel(1);  // 5. Seleciona o Canal 1 (AN1 / RA1)
        delay_us(50);
        valor2 = read_adc(); // 6. Faz a leitura e armazena em 'valor2'
        delay_us(50);
        
        // 7. Mostra o valor2 no LCD
        // (na segunda linha, pois o cursor já pulou com o \n\r anterior)
        printf(lcd_escreve,"A/D value2 = %Lu", valor2);
        
        // --- Código Comentado (Cálculo de Tensão) ---
        // Estas linhas foram deixadas como comentário pelo autor.
        // Elas serviriam para converter o valor digital (0-1023) 
        // de volta para a tensão em Volts (0.00-5.00V).
        tensao =(float)valor*5/1023.;
        printf(lcd_escreve,"A/D (V) = %1.2f",tensao);
        
        // 8. Espera 150ms antes de repetir o loop e atualizar os valores
        delay_ms(150);
    }
}



