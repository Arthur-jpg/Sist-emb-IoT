// #include <WiFi.h>
// #include <PubSubClient.h>

// #define TRIG_PIN 18  // Pino do Trigger
// #define ECHO_PIN 19  // Pino do Echo

// const char* SSID_WIFI = "Juli";       // Nome da rede Wi-Fi
// const char* SENHA_WIFI = "bolo1234678";     // Senha da rede Wi-Fi
// const char* MQTT_SERVER = "test.mosquitto.org"; // Broker MQTT público
// const int MQTT_PORT = 1883;               // Porta MQTT

// WiFiClient espClient;
// PubSubClient client(espClient);

// // Definir altura máxima do recipiente
// const float tamanhoRecipiente = 14.41; // Altere conforme o recipiente (cm)

// // Função para conectar ao Wi-Fi
// void conectarWiFi() {
//   Serial.print("Conectando ao WiFi...");
//   WiFi.begin(SSID_WIFI, SENHA_WIFI);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.print(".");
//   }
//   Serial.println("\nConectado ao WiFi!");
// }

// // Função para conectar ao broker MQTT
// void conectarMQTT() {
//   while (!client.connected()) {
//     Serial.print("Conectando ao MQTT...");
//     if (client.connect("ESP32_HCSR04")) {
//       Serial.println(" Conectado ao broker MQTT!");
//     } else {
//       Serial.print(" Falha! Código: ");
//       Serial.print(client.state());
//       Serial.println(" Tentando novamente em 5 segundos...");
//       delay(5000);
//     }
//   }
// }

// // Função para medir a distância do sensor ultrassônico
// float medirDistancia() {
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);
  
//   long duracao = pulseIn(ECHO_PIN, HIGH);
//   float distancia = duracao * 0.034 / 2; // Conversão para cm
//   return distancia;
// }

// // Configuração inicial do ESP32
// void setup() {
//   Serial.begin(115200);
//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);

//   conectarWiFi();
//   client.setServer(MQTT_SERVER, MQTT_PORT);
// }

// // Loop principal do ESP32
// void loop() {
//   if (!client.connected()) {
//     conectarMQTT();
//   }

//   float distancia = medirDistancia();
//   float calculoNC = tamanhoRecipiente * 0.7; // 70% do recipiente
//   float percentual = (-(distancia / tamanhoRecipiente) * 100) + 100;
//   if (percentual < 0) {
//     percentual = 0;
//   }

//   String status;

//   if (distancia >= tamanhoRecipiente) {
//     status = "Papel acabou";
//   } else if (distancia >= calculoNC) {
//     status = "Nível Crítico";
//   } else {
//     status = "Normal";
//   }

//   // Criar string com os dados formatados
//   String payload = "Status: " + status + "\n";
//   String payload2 = "Distancia em cm: " + String(distancia, 2) + "\n";
//   payload2 += "Porcentagem: " + String(percentual, 2) + "%\n";
  
//   String payload3 = String(percentual); // converte para string

//   // Exibir no Serial Monitor
//   Serial.println("Enviando dados via MQTT:");
//   Serial.println(payload);

//   // Publicar os dados no MQTT
//   client.publish("esp32/hcsr04/status", payload.c_str());
//   client.publish("esp32/hcsr04/dados", payload2.c_str());
//   client.publish("esp32/hcsr04/teste", payload3.c_str()); 

//   client.loop();
//   delay(200); // Atualiza a cada 2 segundos
// }

#include <WiFi.h>
#include <PubSubClient.h>
#include <Ultrasonic.h> // Biblioteca do Erick Simões

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
const float tamanhoRecipiente = 100; // Altere conforme o recipiente (cm)

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
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

// Loop principal
void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }

  float distancia = ultrasonic.read(); // Lê a distância em centímetros
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
  String payload = "Status: " + status + "\n";
  String payload2 = "Distancia em cm: " + String(distancia, 2) + "\n";
  payload2 += "Porcentagem: " + String(percentual, 2) + "%\n";
  String payload3 = String(percentual);

  // Envio via MQTT
  Serial.println("Enviando dados via MQTT:");
  Serial.println(payload);

  client.publish("esp32/hcsr04/status", payload.c_str());
  client.publish("esp32/hcsr04/dados", payload2.c_str());
  client.publish("esp32/hcsr04/teste", payload3.c_str());

  client.loop();
  delay(200); // Ajuste conforme a frequência desejada
}

