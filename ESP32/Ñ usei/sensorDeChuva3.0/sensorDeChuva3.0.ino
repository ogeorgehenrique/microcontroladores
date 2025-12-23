#include <WiFi.h>
#include <WebServer.h>

// --- CONFIGURAÇÕES DE REDE ---
const char* ssid = "Jorgemar";     // Coloque o nome EXATO do Wi-Fi
const char* password = "jerico12";         // Coloque a senha EXATA

// --- PINOS ---
const int pinoSensor = 36; // VP
const int pinoLED = 26;

// --- OBJETOS ---
WebServer server(80); // Cria o servidor na porta padrão 80

// --- PÁGINA HTML (O Site que o ESP32 vai enviar) ---
// Usamos a biblioteca Highcharts (gratuita para uso pessoal) para desenhar o gráfico
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Monitor de Chuva ESP32</title>
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <style>
    body { min-width: 310px; max-width: 800px; height: 400px; margin: 0 auto; font-family: Arial; text-align: center; }
    h2 { color: #042e6f; }
  </style>
</head>
<body>
  <h2>Monitoramento de Chuva em Tempo Real</h2>
  <div id="chart-chuva" class="container"></div>
  <script>
    var chartT = new Highcharts.Chart({
      chart:{ renderTo : 'chart-chuva' },
      title: { text: 'Nivel de Chuva' },
      series: [{
        name: 'Umidade',
        data: []
      }],
      plotOptions: {
        line: { animation: false, dataLabels: { enabled: true } },
        series: { color: '#059e8a' }
      },
      xAxis: { type: 'datetime', dateTimeLabelFormats: { second: '%H:%M:%S' } },
      yAxis: {
        title: { text: 'Intensidade (0-4095)' },
        min: 0, max: 4100
      },
      credits: { enabled: false }
    });

    // Função que pede dados ao ESP32 a cada 2 segundos
    setInterval(function ( ) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var x = (new Date()).getTime(), // Hora atual
              y = parseInt(this.responseText); // Valor do sensor
          
          // Adiciona o ponto no gráfico
          if(chartT.series[0].data.length > 40) {
            chartT.series[0].addPoint([x, y], true, true, true);
          } else {
            chartT.series[0].addPoint([x, y], true, false, true);
          }
        }
      };
      xhttp.open("GET", "/chuva", true);
      xhttp.send();
    }, 2000 ) ; // 2000ms = 2 segundos
  </script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(pinoSensor, INPUT);
  pinMode(pinoLED, OUTPUT);

  // Conecta ao Wi-Fi
  Serial.print("Conectando em ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Aguarda conectar
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Mostra o IP no Monitor Serial
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("Endereço IP para acessar: http://");
  Serial.println(WiFi.localIP());

  // Define as rotas do servidor
  // 1. Quando alguém acessar a raiz (/), envia o HTML
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });

  // 2. Quando o JavaScript pedir (/chuva), envia só o valor numérico
  server.on("/chuva", HTTP_GET, []() {
    int valor = analogRead(pinoSensor);
    int grafico = 4095 - valor; // Inverte valor
    
    // Controle do LED
    if (grafico > 1000) digitalWrite(pinoLED, HIGH);
    else digitalWrite(pinoLED, LOW);

    // Envia o valor para o navegador
    server.send(200, "text/plain", String(grafico));
  });

  server.begin();
}

void loop() {
  server.handleClient(); // Mantém o servidor ouvindo os clientes
}