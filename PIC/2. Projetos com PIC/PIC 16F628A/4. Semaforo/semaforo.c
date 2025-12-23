/*
 * ==============================================================================
 * PROJETO: Cronômetro Regressivo (20 a 0) com Acionamento por Botão
 * PLACA ALVO: McLab 1 (PICSIMLab)
 * MICROCONTROLADOR: PIC16F628A
 *
 * DESCRIÇÃO:
 * Este código implementa um cronômetro de contagem regressiva.
 * O programa começa em um estado de "espera".
 * Ao pressionar o botão em PIN_A1 (B2 na McLab 1), um LED em PIN_A0 (L1)
 * pisca rapidamente e a contagem regressiva é iniciada nos displays.
 * A contagem começa em "20" e desce até "00".
 * Cada número muda a cada 0.2 segundos (200ms).
 * Após chegar ao "00", o display apaga e o programa volta ao
 * estado de "espera", aguardando um novo acionamento do botão.
 * ==============================================================================
 */


#include <16F628A.h> // Biblioteca padrão do PIC16F628A

// --- Configurações do Microcontrolador (FUSES) ---
#FUSES NOWDT         // Desabilita o Watch Dog Timer
#FUSES NOBROWNOUT    // Desabilita o Brownout Reset
#FUSES NOLVP         // Desabilita o Low Voltage Programming

// --- Configuração do Clock (Velocidade) ---
#use delay(crystal=20000000) // Informa ao compilador que estamos usando um Cristal de 20MHz
                            // para que os delays (delay_ms) sejam calculados corretamente.

#define DELAY 1000 // Constante definida mas não utilizada neste código.

signed int8 flag = 1; // Variável global. É usada na lógica de subtração
                      // (cont = cont - flag) na função teste().

// --- Mapas de Bits para os Displays de 7 Segmentos (para McLab 1) ---
// Contêm os padrões de bits para ligar os segmentos corretos E
// controlar qual display (dezena ou unidade) está aceso.
byte const unidade[11] = {
    0b11111110, //0
    0b00111000, //1
    0b11011101, //2
    0b01111101, //3
    0b00111011, //4
    0b01110111, //5
    0b11110111, //6
    0b00111100, //7
    0b11111111, //8
    0b01111111  //9
};

byte const dezena[11] = {
    0b11101110, //0
    0b00101000, //1
    0b11001101, //2
    0b01101101, //3
    0b00101011, //4
    0b01100111, //5
    0b11100111, //6
    0b00101100, //7
    0b11101111, //8
    0b01101111  //9
};

/*
 * ==============================================================================
 * FUNÇÃO: teste()
 * DESCRIÇÃO: Esta função é a "AÇÃO" do programa.
 * Ela é chamada pela 'main()' quando o botão é pressionado.
 * Sua responsabilidade é realizar a contagem regressiva de 20 até 0
 * nos displays de 7 segmentos.
 * ==============================================================================
 */
void teste()
{
    // 1. Inicia as variáveis locais da contagem
    unsigned int i = 0, tempo;
    unsigned int cont = 20; // O contador 'cont' é iniciado em 20.

    // 2. Entra em um loop próprio, que só será interrompido pelo 'break;'
    while (true)
    {

        // 3. Loop de MULTIPLEXAÇÃO (Display)
        // Este loop 'for' roda 100 vezes.
        // Cálculo do tempo: 100 ciclos * (1ms + 1ms) = 200 milissegundos (ou 0.2 segundos)
        // O resultado é que cada número (20, 19, 18...) fica
        // visível no display por 0.2 segundos.
        for (tempo = 0; tempo < 100; tempo++)
        {
            // Acende o display da DEZENA
            i = cont / 10;        // Pega a dezena (ex: 19 / 10 = 1)
            output_b(dezena[i]);  // Envia o padrão do número para o PORTB
            delay_ms(1);          // 4. Delay muito curto (1ms)

            // Acende o display da UNIDADE
            i = cont % 10;        // Pega a unidade (ex: 19 % 10 = 9)
            output_b(unidade[i]); // Envia o padrão do número para o PORTB
            delay_ms(1);          // 4. Delay muito curto (1ms)
        }
        // --- Fim do loop do display ---
        // 5. Neste ponto, 0.2 segundos se passaram.

        // 6. Lógica da Contagem Regressiva
        // Subtrai o valor de 'flag' (que é 1) do contador.
        // Isso é o mesmo que fazer 'cont = cont - 1'
        cont = cont - flag;

        // 7. Condição de Parada
        // O programa verifica se o contador chegou a -1.
        // Isso acontece logo após o display mostrar "00" (quando cont=0).
        // No ciclo seguinte, 'cont' vira -1.
        if (cont == -1)
        {
            // 8. "Quebra" o loop 'while(true)' e SAI da função 'teste()'.
            // Ao sair, o controle do programa volta para a função 'main()',
            // de onde ela foi chamada.
            break;
        }

        /* 9. A lógica antiga do "ping-pong" está COMENTADA.
              O compilador ignora tudo que está dentro de /* ... */
        /*
        if(cont>=20 || cont<=0){
            flag=flag*(-1);
        }
        */
    }
} // Fim da função teste()


/*
 * ==============================================================================
 * FUNÇÃO: main()
 * DESCRIÇÃO: Esta é a função principal, o "GATILHO" do programa.
 * É aqui que o microcontrolador começa a executar o código.
 * Ela fica em um loop infinito, apenas esperando que o botão
 * seja pressionado para então chamar a função 'teste()'.
 * ==============================================================================
 */
void main()
{
    // Loop principal infinito
    while (true)
    {
        // 1. (Opcional) Apenas define o pino RA6 como alto.
        // Na McLab 1, este pino não está conectado a nada visível (LED/Display).
        output_high(pin_a6);

        // 2. Verifica constantemente o estado do botão
        // A função input(pin_a1) lê o pino RA1 (Botão B2 na McLab 1).
        // Se o botão for pressionado, o pino vai para nível 0.
        if (input(pin_a1) == 0)
        {
            // 3. Se o botão foi pressionado, o código entra aqui:

            // 4. Acende o LED L1 (conectado ao pino RA0)
            output_high(pin_a0);

            // 5. Espera 100 milissegundos (para o piscar ser visível)
            delay_ms(100);

            // 6. Apaga o LED L1
            output_low(pin_a0);

            // 7. CHAMA A FUNÇÃO "teste()"
            // O programa "pula" para a função teste() e executa
            // toda a contagem regressiva (de 20 a 0).
            teste();

            // 8. Quando a função teste() termina (após o 'break;'),
            // o programa retorna para esta linha e continua o loop 'while(true)'
            // da 'main()', voltando ao estado de espera (passo 2).
        }
    } // Fim do while(true)
} // Fim da main()
