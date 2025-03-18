#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int sensorPin = 34; // Pino do sensor de peso (ajuste conforme necessário)
const float peso_limite = 100.0; // Limite para considerar que restam poucos rolos

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi!");

    client.setServer(mqtt_server, 1883);
    while (!client.connected()) {
        Serial.println("Conectando ao MQTT...");
        if (client.connect("ESP32Client")) {
            Serial.println("Conectado ao MQTT!");
        } else {
            Serial.print("Falha, rc=");
            Serial.print(client.state());
            Serial.println(" Tentando novamente...");
            delay(5000);
        }
    }
}

void loop() {
    float peso = analogRead(sensorPin); // Simulação do peso
    Serial.print("Peso atual: ");
    Serial.println(peso);

    if (peso < peso_limite) {
        Serial.println("⚠️ Poucos rolos restantes! Enviando alerta...");
        client.publish("estoque/aviso", "ALERTA: Apenas 1 rolo de papel higiênico restante!");
        delay(5000); // Aguarda 5 segundos antes de enviar novamente
    }
    
    delay(2000);
}
