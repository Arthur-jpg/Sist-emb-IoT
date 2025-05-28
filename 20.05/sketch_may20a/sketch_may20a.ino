#define TRIG_PIN 10
#define ECHO_PIN 11

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



