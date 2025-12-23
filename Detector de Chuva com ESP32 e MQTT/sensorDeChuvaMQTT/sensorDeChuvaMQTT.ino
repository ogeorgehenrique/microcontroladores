#include <WiFi.h>
#include <PubSubClient.h>

// --- CONFIGURAÇÕES DE WI-FI ---
const char* ssid = "GEORGE";
const char* password = "gege1234";

// --- CONFIGURAÇÕES MQTT ---
const char* mqtt_server = "broker.hivemq.com"; // Broker público gratuito
const int mqtt_port = 1883;

// Tópicos
const char* topic_publish = "george/sensor/chuva"; 
const char* topic_subscribe = "george/sensor/led";

// --- PINOS ---
const int pinoSensor = 36; // VP
const int pinoLED = 26;

WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis de tempo (para não usar delay)
unsigned long lastMsg = 0;
#define MSG_INTERVAL 2000 // Enviar dados a cada 2 segundos

void setup() {
  Serial.begin(115200);
  pinMode(pinoSensor, INPUT);
  pinMode(pinoLED, OUTPUT);

  setup_wifi();
  
  // Configura o servidor MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); // Define a função que roda quando chega mensagem
}

void loop() {
  // Garante que o MQTT está conectado
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Mantém a comunicação viva

  // Lógica de envio de dados (Substitui o delay)
  unsigned long now = millis();
  if (now - lastMsg > MSG_INTERVAL) {
    lastMsg = now;
    
    // Leitura do Sensor
    int valor = analogRead(pinoSensor);
    int nivelChuva = 4095 - valor; // Inverte valor

    // Prepara a mensagem
    char msg[50];
    snprintf(msg, 50, "%d", nivelChuva);
    
    Serial.print("Publicando: ");
    Serial.println(msg);
    
    // Envia para o Broker
    client.publish(topic_publish, msg);
  }
}

// --- FUNÇÕES AUXILIARES ---

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado!");
}

// Esta função roda AUTOMATICAMENTE quando chega uma mensagem no tópico assinado
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem chegou [");
  Serial.print(topic);
  Serial.print("] ");
  
  // Converte o payload para char para podermos ler
  char comando = (char)payload[0];
  Serial.println(comando);

  // Controle do LED via MQTT
  if (comando == '1') {
    digitalWrite(pinoLED, HIGH);
    Serial.println("LED LIGADO via MQTT");
  } else if (comando == '0') {
    digitalWrite(pinoLED, LOW);
    Serial.println("LED DESLIGADO via MQTT");
  }
}

void reconnect() {
  // Loop até reconectar
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    // Cria um ID randômico para não dar conflito
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      // Assim que conectar, avisa e se inscreve no tópico de comando
      client.publish(topic_publish, "Conectado!");
      client.subscribe(topic_subscribe);
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}