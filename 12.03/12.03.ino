#include <WiFi.h>
#include <PubSubClient.h>

// Configuração da rede Wi-Fi
const char* ssid = "Wifizonee";        // Nome da rede Wi-Fi
const char* password = "sistemasembarcados";  // Senha da rede Wi-Fi

// Configuração do Broker MQTT (computador local)
const char* mqtt_server = "127.0.0.1"; // IP do PC com Mosquitto

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  
  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  // Configurar o broker MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  reconnect();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
      client.subscribe("esp32/test");  // Inscreve-se no tópico
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publica uma mensagem a cada 5 segundos
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    String msg = "Olá do ESP32!";
    client.publish("esp32/test", msg.c_str());
    Serial.println("Mensagem enviada!");
  }
}
