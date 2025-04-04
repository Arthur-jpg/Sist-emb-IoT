#include <WiFi.h>
#include <PubSubClient.h>

#define TRIG_PIN 18  // Pino do Trigger
#define ECHO_PIN 19  // Pino do Echo

const char* SSID_WIFI = "Juli";       // Nome da rede Wi-Fi
const char* SENHA_WIFI = "bolo1234678";     // Senha da rede Wi-Fi
const char* MQTT_SERVER = "test.mosquitto.org"; // Broker MQTT público
const int MQTT_PORT = 1883;               // Porta padrão MQTT

WiFiClient espClient;
PubSubClient client(espClient);

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
      Serial.println("Conectado ao broker MQTT!");
    } else {
      Serial.print("Falha! Código: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// Função para medir a distância usando o sensor HC-SR04
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia = duracao * 0.034 / 2; // Conversão para cm
  return distancia;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  conectarWiFi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }
  
  float distancia = medirDistancia();
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  char msg[10];
  dtostrf(distancia, 4, 2, msg);  // Converte float para string
  
  client.publish("esp32/hcsr04/distance", msg); // Publica no tópico MQTT
  
  client.loop();
  delay(2000); // Envia a cada 2 segundos
}
