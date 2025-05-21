#include <WiFi.h>
#include <PubSubClient.h>
#include <Ultrasonic.h> // Biblioteca do Erick Simões
#include <math.h>

#define TRIG_PIN 18  // Pino do Trigger
#define ECHO_PIN 19  // Pino do Echo

const char* SSID_WIFI = "Teste";       // Nome da rede Wi-Fi
const char* SENHA_WIFI = "eduw8101";     // Senha da rede Wi-Fi
const char* MQTT_SERVER = "test.mosquitto.org"; // Broker MQTT público
const int MQTT_PORT = 1883;               // Porta MQTT

WiFiClient espClient;
PubSubClient client(espClient);

// Instância do sensor ultrassônico
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

// Definir altura máxima do recipiente
// const float tamanhoRecipiente = 14.41; // Altere conforme o recipiente (cm)
const float tamanhoRecipiente = 100; 

// Função para conectar ao Wi-Fi
void conectarWiFi() {
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(SSID_WIFI, SENHA_WIFI);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi!");
}

// Função para conectar ao broker MQTT
void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32_HCSR04")) {
      Serial.println(" Conectado ao broker MQTT!");
    } else {
      Serial.print(" Falha! Código: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// Setup inicial
void setup() {
  Serial.begin(115200);
  conectarWiFi();
  client.setServer(MQTT_SERVER, MQTT_PORT); // Botão para análise
}

// Parâmetros para detecção de falha
#define NUM_LEITURAS 20
#define LIMIAR_FALHA 10.0 // cm, diferença máxima aceitável da média

float leituras[NUM_LEITURAS];
int idxLeitura = 0;
int totalLeituras = 0;
int falhasDetectadas = 0;

float calcularMedia() {
  float soma = 0;
  int n = totalLeituras < NUM_LEITURAS ? totalLeituras : NUM_LEITURAS;
  for (int i = 0; i < n; i++) {
    soma += leituras[i];
  }
  return (n > 0) ? soma / n : 0;
}

// --- Novos parâmetros para análise de erro ---
#define MAX_MEDICOES 100  // Ajuste conforme a RAM disponível
float historicoDistancias[MAX_MEDICOES];
unsigned long historicoLoops[MAX_MEDICOES];
int totalMedicoes = 0;
unsigned long loopCounter = 0;

void calcularEstatisticas() {
  if (totalMedicoes == 0) {
    Serial.println("Nenhuma medição registrada.");
    return;
  }
  float soma = 0;
  for (int i = 0; i < totalMedicoes; i++) {
    soma += historicoDistancias[i];
  }
  float media = soma / totalMedicoes;

  float somaQuadrados = 0;
  for (int i = 0; i < totalMedicoes; i++) {
    somaQuadrados += pow(historicoDistancias[i] - media, 2);
  }
  float desvioPadrao = sqrt(somaQuadrados / totalMedicoes);

  Serial.println("===== Estatísticas do Sensor =====");
  Serial.print("Total de loops: ");
  Serial.println(loopCounter);
  Serial.print("Total de medições: ");
  Serial.println(totalMedicoes);
  Serial.print("Média das distâncias: ");
  Serial.print(media, 4);
  Serial.println(" cm");
  Serial.print("Desvio padrão (erro): ");
  Serial.print(desvioPadrao, 4);
  Serial.println(" cm");
  Serial.println("==================================");
}

void exportarCSV() {
  Serial.println("loop,distancia_cm");
  for (int i = 0; i < totalMedicoes; i++) {
    Serial.print(historicoLoops[i]);
    Serial.print(",");
    Serial.println(historicoDistancias[i], 4);
  }
  Serial.println("FIM_CSV");
}

void loop() {
  loopCounter++; // Conta cada iteração do loop

  if (!client.connected()) {
    conectarMQTT();
  }

  float distancia = ultrasonic.read(); // Lê a distância em centímetros

  // Armazena a leitura no buffer circular
  leituras[idxLeitura] = distancia;
  idxLeitura = (idxLeitura + 1) % NUM_LEITURAS;
  if (totalLeituras < NUM_LEITURAS) totalLeituras++;

  // --- Armazena todas as leituras para análise posterior ---
  if (totalMedicoes < MAX_MEDICOES) {
    historicoDistancias[totalMedicoes] = distancia;
    historicoLoops[totalMedicoes] = loopCounter;
    totalMedicoes++;
  }

  float media = calcularMedia();

  // Detecta falha se a leitura atual divergir muito da média
  bool falha = false;
  if (totalLeituras > 5 && abs(distancia - media) > LIMIAR_FALHA) {
    falhasDetectadas++;
    falha = true;
    // Serial.print("Falha detectada! Distância: ");
    // Serial.print(distancia, 2);
    // Serial.print(" cm, Média: ");
    // Serial.print(media, 2);
    // Serial.println(" cm");
  }

  float taxaFalha = (totalLeituras > 0) ? (float)falhasDetectadas / totalLeituras * 100.0 : 0;

  // ...cálculo de percentual e status...

  float calculoNC = tamanhoRecipiente * 0.7; // 70% do recipiente
  float percentual = (-(distancia / tamanhoRecipiente) * 100) + 100;
  if (percentual < 0) {
    percentual = 0;
  }

  String status;
  if (distancia >= tamanhoRecipiente) {
    status = "Papel acabou";
  } else if (distancia >= calculoNC) {
    status = "Nível Crítico";
  } else {
    status = "Normal";
  }

  // Dados formatados
  // String payload = "Status: " + status + "\n";
  // String payload2 = "Distancia em cm: " + String(distancia, 2) + "\n";
  // payload2 += "Porcentagem: " + String(percentual, 2) + "%\n";
  // String payload3 = String(percentual);

  // Envio via MQTT
  // Serial.println("Enviando dados via MQTT:");
  // Serial.println(payload);

  // Mostra informações de média e taxa de falha no Serial Monitor
  // Serial.print("Média das leituras: ");
  // Serial.print(media, 2);
  // Serial.print(" cm | Falhas detectadas: ");
  // Serial.print(falhasDetectadas);
  // Serial.print(" | Taxa de falha: ");
  // Serial.print(taxaFalha, 2);
  // Serial.println("%");

  // client.publish("esp32/hcsr04/status", payload.c_str());
  // client.publish("esp32/hcsr04/dados", payload2.c_str());
  // client.publish("esp32/hcsr04/teste", payload3.c_str());

  client.loop();
  delay(200); // Ajuste conforme a frequência desejada

  // Imprime apenas a distância com 4 casas decimais para máxima precisão
  Serial.println(distancia, 4);

  // if (loopCounter % 1000 == 0 && totalMedicoes > 0) {
  //   calcularEstatisticas();
  //   exportarCSV();
  //   while (true) {
  //     delay(1000); // Parado aqui para não continuar o loop
  //   }
  // }
}

