#include <Servo.h>
Servo myservo; 

#define trigPin 9
#define echoPin 10
#define servoPin 5

int pos = 0;
int direccion = 1;
void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
 myservo.attach(servoPin);
 Serial.begin(19200);
}

void loop() {
  
  
  //Serial.print("->");
   long duracion, distancia ;

  digitalWrite(trigPin, LOW);        // Nos aseguramos de que el trigger está desactivado
  delayMicroseconds(2);              // Para asegurarnos de que el trigger esta LOW
  digitalWrite(trigPin, HIGH);       // Activamos el pulso de salida
  delayMicroseconds(10);             // Esperamos 10µs. El pulso sigue active este tiempo
  digitalWrite(trigPin, LOW);        // Cortamos el pulso y a esperar el echo

  duracion = pulseIn(echoPin, HIGH) ;
  distancia = duracion / 2 / 29.1  ;

if (distancia > 450) distancia = 450;
myservo.write(pos);
  //Serial.print(pos);
  //Serial.print("-");
  Serial.println(distancia);

  if (direccion == 1) pos++;
  if (direccion == 2) pos--;

  if (pos == 0) direccion = 1;
  if (pos == 180) direccion = 2;
  delay(500);
}
