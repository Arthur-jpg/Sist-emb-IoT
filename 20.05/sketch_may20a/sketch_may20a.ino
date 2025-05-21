//Printando tudo
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <Ultrasonic.h> // Biblioteca do Erick Simões
// #include <math.h>

// #define TRIG_PIN 18  // Pino do Trigger
// #define ECHO_PIN 19  // Pino do Echo

// const char* SSID_WIFI = "Teste";       // Nome da rede Wi-Fi
// const char* SENHA_WIFI = "eduw8101";     // Senha da rede Wi-Fi
// const char* MQTT_SERVER = "test.mosquitto.org"; // Broker MQTT público
// const int MQTT_PORT = 1883;               // Porta MQTT

// WiFiClient espClient;
// PubSubClient client(espClient);

// // Instância do sensor ultrassônico
// Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

// // Definir altura máxima do recipiente
// // const float tamanhoRecipiente = 14.41; // Altere conforme o recipiente (cm)
// const float tamanhoRecipiente = 100; 

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

// // Setup inicial
// void setup() {
//   Serial.begin(115200);
//   conectarWiFi();
//   client.setServer(MQTT_SERVER, MQTT_PORT); // Botão para análise
// }

// // Parâmetros para detecção de falha
// #define NUM_LEITURAS 20
// #define LIMIAR_FALHA 10.0 // cm, diferença máxima aceitável da média

// float leituras[NUM_LEITURAS];
// int idxLeitura = 0;
// int totalLeituras = 0;
// int falhasDetectadas = 0;

// float calcularMedia() {
//   float soma = 0;
//   int n = totalLeituras < NUM_LEITURAS ? totalLeituras : NUM_LEITURAS;
//   for (int i = 0; i < n; i++) {
//     soma += leituras[i];
//   }
//   return (n > 0) ? soma / n : 0;
// }

// // --- Novos parâmetros para análise de erro ---
// #define MAX_MEDICOES 500  // Ajuste conforme a RAM disponível
// float historicoDistancias[MAX_MEDICOES];
// unsigned long historicoLoops[MAX_MEDICOES];
// int totalMedicoes = 0;
// unsigned long loopCounter = 0;

// void calcularEstatisticas() {
//   if (totalMedicoes == 0) {
//     Serial.println("Nenhuma medição registrada.");
//     return;
//   }
//   float soma = 0;
//   for (int i = 0; i < totalMedicoes; i++) {
//     soma += historicoDistancias[i];
//   }
//   float media = soma / totalMedicoes;

//   float somaQuadrados = 0;
//   for (int i = 0; i < totalMedicoes; i++) {
//     somaQuadrados += pow(historicoDistancias[i] - media, 2);
//   }
//   float desvioPadrao = sqrt(somaQuadrados / totalMedicoes);

//   Serial.println("===== Estatísticas do Sensor =====");
//   Serial.print("Total de loops: ");
//   Serial.println(loopCounter);
//   Serial.print("Total de medições: ");
//   Serial.println(totalMedicoes);
//   Serial.print("Média das distâncias: ");
//   Serial.print(media, 4);
//   Serial.println(" cm");
//   Serial.print("Desvio padrão (erro): ");
//   Serial.print(desvioPadrao, 4);
//   Serial.println(" cm");
//   Serial.println("==================================");
// }

// void exportarCSV() {
//   Serial.println("loop,distancia_cm");
//   for (int i = 0; i < totalMedicoes; i++) {
//     Serial.print(historicoLoops[i]);
//     Serial.print(",");
//     Serial.println(historicoDistancias[i], 4);
//   }
//   Serial.println("FIM_CSV");
// }

// void loop() {
//   loopCounter++; // Conta cada iteração do loop

//   if (!client.connected()) {
//     conectarMQTT();
//   }

//   float distancia = ultrasonic.read(); // Lê a distância em centímetros

//   // Armazena a leitura no buffer circular
//   leituras[idxLeitura] = distancia;
//   idxLeitura = (idxLeitura + 1) % NUM_LEITURAS;
//   if (totalLeituras < NUM_LEITURAS) totalLeituras++;

//   // --- Armazena todas as leituras para análise posterior ---
//   if (totalMedicoes < MAX_MEDICOES) {
//     historicoDistancias[totalMedicoes] = distancia;
//     historicoLoops[totalMedicoes] = loopCounter;
//     totalMedicoes++;
//   }

//   float media = calcularMedia();

//   // Detecta falha se a leitura atual divergir muito da média
//   bool falha = false;
//   if (totalLeituras > 5 && abs(distancia - media) > LIMIAR_FALHA) {
//     falhasDetectadas++;
//     falha = true;
//     Serial.print("Falha detectada! Distância: ");
//     Serial.print(distancia, 2);
//     Serial.print(" cm, Média: ");
//     Serial.print(media, 2);
//     Serial.println(" cm");
//   }

//   float taxaFalha = (totalLeituras > 0) ? (float)falhasDetectadas / totalLeituras * 100.0 : 0;

//   // ...cálculo de percentual e status...

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

//   // Dados formatados
//   String payload = "Status: " + status + "\n";
//   String payload2 = "Distancia em cm: " + String(distancia, 2) + "\n";
//   payload2 += "Porcentagem: " + String(percentual, 2) + "%\n";
//   String payload3 = String(percentual);

//   // Envio via MQTT
//   Serial.println("Enviando dados via MQTT:");
//   Serial.println(payload);

//   // Mostra informações de média e taxa de falha no Serial Monitor
//   Serial.print("Média das leituras: ");
//   Serial.print(media, 2);
//   Serial.print(" cm | Falhas detectadas: ");
//   Serial.print(falhasDetectadas);
//   Serial.print(" | Taxa de falha: ");
//   Serial.print(taxaFalha, 2);
//   Serial.println("%");

//   client.publish("esp32/hcsr04/status", payload.c_str());
//   client.publish("esp32/hcsr04/dados", payload2.c_str());
//   client.publish("esp32/hcsr04/teste", payload3.c_str());

//   client.loop();
//   delay(200); // Ajuste conforme a frequência desejada

//   // Imprime estatísticas e exporta CSV a cada 1000 loops, depois encerra o loop
//   if (loopCounter % 1000 == 0 && totalMedicoes > 0) {
//     calcularEstatisticas();
//     exportarCSV();
//     while (true) {
//       delay(1000); // Parado aqui para não continuar o loop
//     }
//   }
// }



// só a distancia porem com mosquitto

// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <math.h>

// #define TRIG_PIN 18  // Pino do Trigger
// #define ECHO_PIN 19  // Pino do Echo

// const char* SSID_WIFI = "Teste";
// const char* SENHA_WIFI = "eduw8101";
// const char* MQTT_SERVER = "test.mosquitto.org";
// const int MQTT_PORT = 1883;

// WiFiClient espClient;
// PubSubClient client(espClient);

// // --- Precisão de leitura em microssegundos ---
// float lerDistanciaCm() {
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);

//   long duracao = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout: 30ms (máx ~5m)
//   if (duracao == 0) return -1.0; // Falha na leitura

//   float distancia = (duracao / 2.0) * 0.0343; // cm com precisão
//   return distancia;
// }

// // Altura do recipiente
// const float tamanhoRecipiente = 100.0;

// void conectarWiFi() {
//   Serial.print("Conectando ao WiFi...");
//   WiFi.begin(SSID_WIFI, SENHA_WIFI);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.print(".");
//   }
//   Serial.println("\nConectado ao WiFi!");
// }

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

// void setup() {
//   Serial.begin(115200);
//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);
//   conectarWiFi();
//   client.setServer(MQTT_SERVER, MQTT_PORT);
// }

// // --- Parâmetros de leitura e falhas ---
// #define NUM_LEITURAS 20
// #define LIMIAR_FALHA 10.0

// float leituras[NUM_LEITURAS];
// int idxLeitura = 0;
// int totalLeituras = 0;
// int falhasDetectadas = 0;

// float calcularMedia() {
//   float soma = 0;
//   int n = totalLeituras < NUM_LEITURAS ? totalLeituras : NUM_LEITURAS;
//   for (int i = 0; i < n; i++) {
//     soma += leituras[i];
//   }
//   return (n > 0) ? soma / n : 0;
// }

// // --- Histórico ---
// #define MAX_MEDICOES 100
// float historicoDistancias[MAX_MEDICOES];
// unsigned long historicoLoops[MAX_MEDICOES];
// int totalMedicoes = 0;
// unsigned long loopCounter = 0;

// void calcularEstatisticas() {
//   if (totalMedicoes == 0) {
//     Serial.println("Nenhuma medição registrada.");
//     return;
//   }
//   float soma = 0;
//   for (int i = 0; i < totalMedicoes; i++) {
//     soma += historicoDistancias[i];
//   }
//   float media = soma / totalMedicoes;

//   float somaQuadrados = 0;
//   for (int i = 0; i < totalMedicoes; i++) {
//     somaQuadrados += pow(historicoDistancias[i] - media, 2);
//   }
//   float desvioPadrao = sqrt(somaQuadrados / totalMedicoes);

//   Serial.println("===== Estatísticas do Sensor =====");
//   Serial.print("Total de loops: ");
//   Serial.println(loopCounter);
//   Serial.print("Total de medições: ");
//   Serial.println(totalMedicoes);
//   Serial.print("Média das distâncias: ");
//   Serial.print(media, 4);
//   Serial.println(" cm");
//   Serial.print("Desvio padrão (erro): ");
//   Serial.print(desvioPadrao, 4);
//   Serial.println(" cm");
//   Serial.println("==================================");
// }

// void exportarCSV() {
//   Serial.println("loop,distancia_cm");
//   for (int i = 0; i < totalMedicoes; i++) {
//     Serial.print(historicoLoops[i]);
//     Serial.print(",");
//     Serial.println(historicoDistancias[i], 4);
//   }
//   Serial.println("FIM_CSV");
// }

// void loop() {
//   loopCounter++;

//   if (!client.connected()) {
//     conectarMQTT();
//   }

//   float distancia = lerDistanciaCm();

//   // Buffer circular
//   leituras[idxLeitura] = distancia;
//   idxLeitura = (idxLeitura + 1) % NUM_LEITURAS;
//   if (totalLeituras < NUM_LEITURAS) totalLeituras++;

//   // Histórico completo
//   if (totalMedicoes < MAX_MEDICOES) {
//     historicoDistancias[totalMedicoes] = distancia;
//     historicoLoops[totalMedicoes] = loopCounter;
//     totalMedicoes++;
//   }

//   float media = calcularMedia();

//   bool falha = false;
//   if (totalLeituras > 5 && abs(distancia - media) > LIMIAR_FALHA) {
//     falhasDetectadas++;
//     falha = true;
//   }

//   float taxaFalha = (totalLeituras > 0) ? (float)falhasDetectadas / totalLeituras * 100.0 : 0;

//   float calculoNC = tamanhoRecipiente * 0.7;
//   float percentual = (-(distancia / tamanhoRecipiente) * 100) + 100;
//   if (percentual < 0) percentual = 0;

//   String status;
//   if (distancia >= tamanhoRecipiente) {
//     status = "Papel acabou";
//   } else if (distancia >= calculoNC) {
//     status = "Nível Crítico";
//   } else {
//     status = "Normal";
//   }

//   // Publicação MQTT (comentado por padrão)
//   /*
//   String payload = "Status: " + status + "\n";
//   String payload2 = "Distancia em cm: " + String(distancia, 4) + "\n";
//   payload2 += "Porcentagem: " + String(percentual, 2) + "%\n";
//   String payload3 = String(percentual);

//   client.publish("esp32/hcsr04/status", payload.c_str());
//   client.publish("esp32/hcsr04/dados", payload2.c_str());
//   client.publish("esp32/hcsr04/teste", payload3.c_str());
//   */

//   client.loop();
//   delay(200);

//   // Imprime a distância com 4 casas decimais
//   Serial.println(distancia, 4);

//   // Estatísticas (descomente se quiser debug após 1000 loops)
//   /*
//   if (loopCounter % 1000 == 0 && totalMedicoes > 0) {
//     calcularEstatisticas();
//     exportarCSV();
//     while (true) {
//       delay(1000);
//     }
//   }
//   */
// }


#define TRIG_PIN 18
#define ECHO_PIN 19

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Envia pulso para o trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Lê o tempo de retorno
  long duracao = pulseIn(ECHO_PIN, HIGH, 30000); // timeout de 30ms (~5m)
  if (duracao == 0) {
    Serial.println("Erro de leitura");
  } else {
    float distancia = (duracao / 2.0) * 0.0343;
    Serial.println(distancia, 4);
  }

  delay(200);
}



