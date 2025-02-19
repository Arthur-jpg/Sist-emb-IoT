#include <Ultrasonic.h>
#define LED1 3
#define Trig 4
#define Echo 5
#define CHAVE 2

const int pinoPOT = A0;

float luminosidadeLed = 0;


Ultrasonic ultrasonic(Trig, Echo);




void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(pinoPOT, INPUT);
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
}

void loop() {

  
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  Serial.println("Distancia em cm: ");
  Serial.println(cmMsec);



  delay(1000);

  luminosidadeLed = map(analogRead(pinoPOT), 0, 1023, 0, 255);
  analogWrite(LED1, luminosidadeLed);


  
}
