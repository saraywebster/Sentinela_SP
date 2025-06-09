#include <WiFi.h>
#include <WebServer.h>      // Servidor web no ESP32
#include <ArduinoJson.h>    // Para formatação JSON

// --- Configurações de WiFi ---
const char* WIFI_SSID = "SeuSSID"; // Substitua pelo SSID da sua rede WiFi
const char* WIFI_SENHA = "SuaSenha"; // Substitua pela senha da sua rede WiFi

// --- Configurações do Servidor Web ---
WebServer server(80);

// --- Pinos dos LEDs ---
const int LED_OPERACIONAL_AZUL = 23;
const int LED_NIVEL_NORMAL_VERDE = 26;
const int LED_NIVEL_PERIGO_AMARELO = 33;
const int LED_RISCO_INUNDACAO_VERMELHO = 21;
const int LED_WIFI_STATUS_BRANCO = 22;

// --- Pino do Sensor de Água ---
const int SENSOR_AGUA_PIN = 34; // Pino analógico

enum EstadoNivelAgua {
  SECO,
  NORMAL,
  PERIGO,
  INUNDACAO
};

// --- Limites do Sensor de Água ---
const int NIVEL_NORMAL_MAX = 1500;
const int NIVEL_PERIGO_MAX = 2500;

// Centraliza o estado atual do sensor para que todas as funções usem o mesmo dado.
// 'volatile' informa ao compilador que a variável pode mudar a qualquer momento.
volatile int g_valorSensorAtual = 0;
volatile EstadoNivelAgua g_estadoAguaAtual = SECO;

// --- Intervalos de Tempo ---
unsigned long proximoTempoLeitura = 0;
const long INTERVALO_LEITURA_SENSOR = 5000; // 5 segundos
unsigned long proximoTempoReconexaoWifi = 0;
const long INTERVALO_RECONEXAO_WIFI = 10000; // 10 segundos

// --- Protótipos das Funções ---
void setup_wifi();
EstadoNivelAgua getEstadoPeloValor(int valorSensor);
const char* getEstadoString(EstadoNivelAgua estado);
void atualizarLedsNivel();
void handleDadosSensor();

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando sistema...");

  pinMode(LED_OPERACIONAL_AZUL, OUTPUT);
  pinMode(LED_NIVEL_NORMAL_VERDE, OUTPUT);
  pinMode(LED_NIVEL_PERIGO_AMARELO, OUTPUT);
  pinMode(LED_RISCO_INUNDACAO_VERMELHO, OUTPUT);
  pinMode(LED_WIFI_STATUS_BRANCO, OUTPUT);
  
  digitalWrite(LED_OPERACIONAL_AZUL, LOW);
  digitalWrite(LED_NIVEL_NORMAL_VERDE, LOW);
  digitalWrite(LED_NIVEL_PERIGO_AMARELO, LOW);
  digitalWrite(LED_RISCO_INUNDACAO_VERMELHO, LOW);
  digitalWrite(LED_WIFI_STATUS_BRANCO, LOW);

  Serial.println("Teste dos LEDs...");
  digitalWrite(LED_OPERACIONAL_AZUL, HIGH); delay(300);
  digitalWrite(LED_NIVEL_NORMAL_VERDE, HIGH); delay(300);
  digitalWrite(LED_NIVEL_PERIGO_AMARELO, HIGH); delay(300);
  digitalWrite(LED_RISCO_INUNDACAO_VERMELHO, HIGH); delay(300);
  digitalWrite(LED_WIFI_STATUS_BRANCO, HIGH); delay(500);
  
  digitalWrite(LED_NIVEL_NORMAL_VERDE, LOW);
  digitalWrite(LED_NIVEL_PERIGO_AMARELO, LOW);
  digitalWrite(LED_RISCO_INUNDACAO_VERMELHO, LOW);
  digitalWrite(LED_WIFI_STATUS_BRANCO, LOW);
  Serial.println("Teste dos LEDs concluído. LED Operacional (Azul) LIGADO.");

  setup_wifi();

  if (WiFi.status() == WL_CONNECTED) {
    server.on("/dados", HTTP_GET, handleDadosSensor);
    server.begin();
    Serial.println("Servidor HTTP iniciado.");
  }

  Serial.println("Setup concluído.");
}

// --- LOOP PRINCIPAL ---
void loop() {
  unsigned long currentMillis = millis();

  // Manter a conexão WiFi e o servidor
  if (WiFi.status() != WL_CONNECTED && currentMillis >= proximoTempoReconexaoWifi) {
    proximoTempoReconexaoWifi = currentMillis + INTERVALO_RECONEXAO_WIFI;
    Serial.println("WiFi desconectado. Tentando reconectar...");
    setup_wifi();
    if (WiFi.status() == WL_CONNECTED) {
        server.begin(); // Reinicia o servidor se reconectou
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  // Ler o sensor e atualizar o estado periodicamente
  if (currentMillis >= proximoTempoLeitura) {
    proximoTempoLeitura = currentMillis + INTERVALO_LEITURA_SENSOR;
    
    // Atualiza as variáveis globais de estado
    g_valorSensorAtual = analogRead(SENSOR_AGUA_PIN);
    g_estadoAguaAtual = getEstadoPeloValor(g_valorSensorAtual);
    
    // Atualiza os LEDs com base no novo estado
    atualizarLedsNivel();
  }

  delay(10); // Pequeno delay para estabilidade geral
}


// --- Funções Auxiliares ---

void setup_wifi() {
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  int tentativas = 0;
  digitalWrite(LED_WIFI_STATUS_BRANCO, LOW); 
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    digitalWrite(LED_WIFI_STATUS_BRANCO, !digitalRead(LED_WIFI_STATUS_BRANCO));
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_WIFI_STATUS_BRANCO, HIGH);
    Serial.println("\nWiFi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    digitalWrite(LED_WIFI_STATUS_BRANCO, LOW);
    Serial.println("\nFalha ao conectar ao WiFi.");
  }
}

// Função que retorna o ENUM correspondente ao valor do sensor
EstadoNivelAgua getEstadoPeloValor(int valorSensor) {
  if (valorSensor < 10) { // Uma pequena margem para "Seco"
    return SECO;
  } else if (valorSensor <= NIVEL_NORMAL_MAX) {
    return NORMAL;
  } else if (valorSensor <= NIVEL_PERIGO_MAX) {
    return PERIGO;
  } else {
    return INUNDACAO;
  }
}

// Função que converte o ENUM para uma string (apenas para JSON e Serial)
const char* getEstadoString(EstadoNivelAgua estado) {
  switch (estado) {
    case SECO: return "Seco";
    case NORMAL: return "Normal";
    case PERIGO: return "Perigo";
    case INUNDACAO: return "Inundacao";
    default: return "Desconhecido";
  }
}

void atualizarLedsNivel() {
  // Apaga todos os LEDs de nível primeiro
  digitalWrite(LED_NIVEL_NORMAL_VERDE, LOW);
  digitalWrite(LED_NIVEL_PERIGO_AMARELO, LOW);
  digitalWrite(LED_RISCO_INUNDACAO_VERMELHO, LOW);

  Serial.print("Valor Sensor: " + String(g_valorSensorAtual) + " - ");
  Serial.print("Nível: ");
  Serial.println(getEstadoString(g_estadoAguaAtual));
  
  switch (g_estadoAguaAtual) {
    case SECO:
    case NORMAL:
      digitalWrite(LED_NIVEL_NORMAL_VERDE, HIGH);
      break;
    case PERIGO:
      digitalWrite(LED_NIVEL_PERIGO_AMARELO, HIGH);
      break;
    case INUNDACAO:
      digitalWrite(LED_RISCO_INUNDACAO_VERMELHO, HIGH);
      break;
  }
}

// Endpoint do servidor usa a variável de estado global ---
// Não faz uma nova leitura do sensor, garantindo consistência.
void handleDadosSensor() {
  JsonDocument jsonDocument;

  jsonDocument["valorSensor"] = g_valorSensorAtual;
  jsonDocument["estado"] = getEstadoString(g_estadoAguaAtual);

  String responseBuffer;
  serializeJson(jsonDocument, responseBuffer);

  server.send(200, "application/json", responseBuffer);
}