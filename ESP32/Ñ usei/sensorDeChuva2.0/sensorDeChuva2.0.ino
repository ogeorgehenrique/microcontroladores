const int pinoSensor = 36; // Sensor de Chuva (VP)
const int pinoLED = 26;    // LED de Alerta

// Ajuste este valor olhando seu gráfico!
// Se passar disso, consideramos que está chovendo.
const int LIMITE_CHUVA = 1000; 

void setup() {
  Serial.begin(115200);
  pinMode(pinoSensor, INPUT);
  pinMode(pinoLED, OUTPUT); // Configura o LED como saída
}

void loop() {
  int valorLeitura = analogRead(pinoSensor);
  int nivelChuva = 4095 - valorLeitura; // Invertendo: 0=Seco, 4095=Molhado

  Serial.print("Nivel_Chuva:");
  Serial.print(nivelChuva);
  Serial.print(","); // A virgula separa as linhas no Plotter
  Serial.print("Limite_Alarme:"); // Mostra uma linha fixa no gráfico para referência
  Serial.println(LIMITE_CHUVA);

  // A Lógica de Decisão
  if (nivelChuva > LIMITE_CHUVA) {
    digitalWrite(pinoLED, HIGH); // Liga o LED
    // Opcional: Imprimir aviso se não estiver usando o Plotter
    // Serial.println(" -> ESTÁ CHOVENDO!"); 
  } else {
    digitalWrite(pinoLED, LOW);  // Desliga o LED
  }

  delay(100);
}