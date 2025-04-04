#define TRIG_PIN 33  // Pino TRIG do sensor
#define ECHO_PIN 32  // Pino ECHO do sensor

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Envia pulso ultrassônico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Mede o tempo do eco
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Converte para centímetros
  float distance = duration * 0.034 / 2;

  // Exibe no monitor serial
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);  // Aguarda 1 segundo para próxima leitura
}
