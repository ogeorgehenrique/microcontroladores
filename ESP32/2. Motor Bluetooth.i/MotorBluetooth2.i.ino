/*
   PROJETO: Controle de Motor CC via Bluetooth Low Energy (BLE)
   COMPATIBILIDADE: iPhone (iOS) e Android
   HARDWARE: ESP32 + Driver Ponte H (L298N)
   
   INSTRUÇÕES DE USO:
   1. Instale o app "RemoteXY".
   2. Conecte no Bluetooth "ESP32_Motor_Final".
   3. Use a interface para controlar o motor.
*/

// =================================================================================
// 1. CONFIGURAÇÕES DE BIBLIOTECAS E BLUETOOTH
// =================================================================================

// Ela força o uso do modo BLE (Bluetooth Low Energy).
// Sem isso, o iPhone não enxerga o ESP32.
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>
#include <RemoteXY.h>

// Nome que vai aparecer na lista de Bluetooth do celular
#define REMOTEXY_BLUETOOTH_NAME "ESP32_Motor_Final"

// --- MAPA DA INTERFACE GRÁFICA ---
// Este array hexadecimal é gerado pelo site RemoteXY.
// Ele diz ao aplicativo onde desenhar os botões e o slider na tela.
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,3,0,0,0,0,0,16,165,1,3,131,1,9,42,21,2,26,2,1,
  131,48,9,42,21,1,26,4,4,130,17,44,57,9,2,26 };

// --- ESTRUTURA DE DADOS (VINCULAÇÃO) ---
// Esta estrutura conecta os botões do celular com variáveis no ESP32.
// Quando você mexe no celular, essas variáveis mudam sozinhas aqui.
struct {
  uint8_t switch_power;   // Botão Ligar/Desligar (0 = Desligado, 1 = Ligado)
  uint8_t switch_sentido; // Chave de Sentido (0 = Direção A, 1 = Direção B)
  int8_t slider_vel;      // Slider de Velocidade (Vai de 0 a 100)
  uint8_t connect_flag;   // Indica se tem alguém conectado (1 = Sim, 0 = Não)
} RemoteXY;
#pragma pack(pop)


// =================================================================================
// 2. DEFINIÇÃO DE HARDWARE (PINOS)
// =================================================================================

// Pinos conectados na Ponte H (L298N)
const int pinoIN1 = 19; // Controla Direção A
const int pinoIN2 = 18; // Controla Direção B
const int pinoENA = 16; // Controla a Velocidade (PWM)

// =================================================================================
// 3. SETUP (CONFIGURAÇÕES INICIAIS)
// =================================================================================
void setup() {
  RemoteXY_Init();       // Inicia o serviço Bluetooth
  Serial.begin(115200);  // Inicia comunicação Serial para Debug no PC
  
  // Configura os pinos de direção como SAÍDA (Output)
  pinMode(pinoIN1, OUTPUT);
  pinMode(pinoIN2, OUTPUT);
  
  // --- CONFIGURAÇÃO DO PWM (Velocidade) ---
  // A função ledcAttach configura o pino para funcionar como PWM.
  // Sintaxe: ledcAttach(PINO, FREQUÊNCIA, RESOLUÇÃO);
  //
  // 1. PINO: pinoENA (GPIO 16)
  // 2. FREQUÊNCIA: 30000 Hz (30 kHz). 
 
  // 3. RESOLUÇÃO: 8 bits.
  //    * 8 bits significa 2^8 = 256 níveis de controle.
  //    * Nossa velocidade irá de 0 (Parado) a 255 (Máxima potência).
  ledcAttach(pinoENA, 30000, 8);
  
  // Garante que o motor comece parado ao ligar a placa
  pararMotor();
  
  Serial.println("Sistema iniciado! Aguardando conexao Bluetooth...");
}


// =================================================================================
// 4. LOOP (LÓGICA DE CONTROLE)
// =================================================================================
void loop() {
 
  RemoteXY_Handler();

  // --- LÓGICA DE LIGAR / DESLIGAR ---
  // Verifica se o interruptor principal está desligado (0)
  if (RemoteXY.switch_power == 0) {
    pararMotor(); // Se desligado, corta tudo.
  } 
  else {
    
    int velocidade = RemoteXY.slider_vel * 2.55;
    
    // --- TRAVAS DE SEGURANÇA ---
    // Garante que a variável nunca ultrapasse os limites do PWM
    if (velocidade > 255) velocidade = 255;
    if (velocidade < 0) velocidade = 0;

    // --- APLICAÇÃO DA VELOCIDADE (DUTY CYCLE) ---
    // A função ledcWrite altera o "Duty Cycle" (Ciclo de Trabalho).

    ledcWrite(pinoENA, velocidade);

    // --- LÓGICA DE SENTIDO (PONTE H) ---
    // Verifica a chave de sentido no app
    if (RemoteXY.switch_sentido == 0) {
      // Sentido Horário (Frente)
      // Para girar, um lado deve ser HIGH e o outro LOW.
      digitalWrite(pinoIN1, HIGH);
      digitalWrite(pinoIN2, LOW);
    } else {
      // Sentido Anti-Horário (Trás)
      // Invertemos a polaridade: IN1 vira LOW e IN2 vira HIGH.
      digitalWrite(pinoIN1, LOW);
      digitalWrite(pinoIN2, HIGH);
    }
  }
  
  // Pequeno atraso para não sobrecarregar o processador
  delay(10);
}

// =================================================================================
// 5. FUNÇÕES AUXILIARES
// =================================================================================


void pararMotor() {
  digitalWrite(pinoIN1, LOW);  // Desliga saida 1
  digitalWrite(pinoIN2, LOW);  // Desliga saida 2
  ledcWrite(pinoENA, 0);       // Zera o PWM (Velocidade 0)
}