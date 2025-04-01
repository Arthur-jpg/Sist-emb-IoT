#include <Ultrasonic.h>
#define Trig 10
#define Echo 9

Ultrasonic ultrasonic(Trig, Echo);

float tamanhoRecipiente = 14.41;

void setup() {
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
}

void loop() {

  float medida;
  float calculoNC = tamanhoRecipiente * 0.7;
  
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  if (cmMsec >= tamanhoRecipiente) {
    Serial.println("Papel acabou");
  } else if (cmMsec >= (calculoNC)) {
    Serial.println("Nível crítico");

  }

  Serial.println("Distancia em cm: ");
  Serial.println(cmMsec);
  Serial.println("Porcentagem");
  Serial.println((-(cmMsec/tamanhoRecipiente) * 100) + 100 );




  delay(1000);



  
}