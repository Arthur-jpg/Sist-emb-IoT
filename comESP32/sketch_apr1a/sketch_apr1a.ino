#include <WiFi.h>
#include <PubSubClient.h>

// Definição dos pinos do sensor ultrassônico
#define TRIG_PIN 10
#define ECHO_PIN 9

// Configuração do WiFi
const char* ssid = "Juli";       // Substitua pelo nome da sua rede WiFi
const char* password = "bolo1234678";  // Substitua pela senha do WiFi

// Configuração do Broker MQTT
const char* mqtt_server = "test.mosquitto.org";  // Broker MQTT público
const char* mqtt_topic = "estoque/esp32/ibmec";    // Tópico MQTT para envio dos dados

WiFiClient espClient;
PubSubClient client(espClient);

float tamanhoRecipiente = 14.41;

void setup_wifi() {
  delay(10);
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Função de callback (não usada neste exemplo)
void callback(char* topic, byte* payload, unsigned int length) {}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Medir a distância usando o sensor ultrassônico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  float cmMsec = duration * 0.034 / 2;  // Conversão para cm

  // Cálculo do nível crítico
  float calculoNC = tamanhoRecipiente * 0.7;
  String message;

  if (cmMsec >= tamanhoRecipiente) {
    message = "Papel acabou";
  } else if (cmMsec >= calculoNC) {
    message = "Nível crítico";
  } else {
    message = "Distância em cm: " + String(cmMsec);
  }

  float porcentagem = (-(cmMsec / tamanhoRecipiente) * 100) + 100;
  message += " | Porcentagem: " + String(porcentagem) + "%";

  Serial.println(message);
  client.publish(mqtt_topic, message.c_str());

  delay(1000);
}
