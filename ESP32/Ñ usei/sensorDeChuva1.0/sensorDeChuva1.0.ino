// Definição do pino
const int pinoSensor = 36; // Pino VP (GPIO 36)

void setup() {
  // Inicia a comunicação serial
  Serial.begin(115200);
  
  // No ESP32 não é estritamente necessário definir pinMode para analogRead,
  // mas é uma boa prática em algumas versões.
  pinMode(pinoSensor, INPUT);
}

void loop() {
  // O ESP32 tem resolução de 12 bits (vai de 0 a 4095)
  int valorLeitura = analogRead(pinoSensor);

  // Inverte o valor para o gráfico ficar mais intuitivo
  // (Assim: Barra alta = Muita chuva / Barra baixa = Seco)
  // Se não inverter: 4095 seria seco e 0 seria encharcado
  int graficoChuva = 4095 - valorLeitura;

  // Imprime os valores para o Plotter
  // Texto "Chuva:" serve como legenda da linha no gráfico
  Serial.print("Chuva:"); 
  Serial.println(graficoChuva);

  // Pequena pausa para o gráfico não correr rápido demais
  delay(100); 
}