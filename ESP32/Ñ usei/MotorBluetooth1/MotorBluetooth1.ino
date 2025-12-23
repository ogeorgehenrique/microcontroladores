/*
   PROJETO: Controle de Motor CC via Bluetooth LE (Corrigido para ESP32 v3.0)
*/

// --- INÍCIO DA CONFIGURAÇÃO REMOTEXY ---
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY2"



// Configuração da Interface (Botão, Switch e Slider) - IGUAL AO ANTERIOR
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,3,0,0,0,0,0,16,165,1,3,131,1,9,42,21,2,26,2,1,
  131,48,9,42,21,1,26,4,4,130,17,44,57,9,2,26 };

struct {
  // input variables
  uint8_t Botao_power; // =1 if switch ON and =0 if OFF, from 0 to 1
  uint8_t optionSelector_01; // from 0 to 2
  int8_t Velocidade; // from 0 to 100
   // Flag de conexão
} RemoteXY;
#pragma pack(pop)
// --- FIM DA CONFIGURAÇÃO REMOTEXY ---


// --- SEUS PINOS (L298N) ---
const int pinoIN1 = 18; 
const int pinoIN2 = 29; 
const int pinoENA = 16; 

// Na versão 3.0 não precisamos mais definir canais manualmente, o ESP resolve sozinho

void setup() {
  RemoteXY_Init(); 
  Serial.begin(115200);
  
  pinMode(pinoIN1, OUTPUT);
  pinMode(pinoIN2, OUTPUT);
  
  // CORREÇÃO AQUI: Novo comando da versão 3.0
  // Sintaxe: ledcAttach(pino, frequencia, resolução)
  ledcAttach(pinoENA, 30000, 8);
  
  // Começa parado
  pararMotor();
}

void loop() {
  RemoteXY_Handler();

  // Verifica se está ligado (switch_power == 1)
  if (RemoteXY.switch_power == 0) {
    pararMotor();
  } 
  else {
    // Calcula velocidade (0-100 para 0-255)
    int velocidade = RemoteXY.slider_vel * 2.55;
    if (velocidade > 255) velocidade = 255;
    if (velocidade < 0) velocidade = 0;

    // CORREÇÃO AQUI: Escrevemos direto no PINO, não no canal
    ledcWrite(pinoENA, velocidade);

    // Verifica sentido (switch_sentido)
    if (RemoteXY.switch_sentido == 0) {
      digitalWrite(pinoIN1, HIGH);
      digitalWrite(pinoIN2, LOW);
    } else {
      digitalWrite(pinoIN1, LOW);
      digitalWrite(pinoIN2, HIGH);
    }
  }
  delay(10);
}

void pararMotor() {
  digitalWrite(pinoIN1, LOW);
  digitalWrite(pinoIN2, LOW);
  // CORREÇÃO AQUI: Escreve zero no pino para parar o PWM
  ledcWrite(pinoENA, 0);
}