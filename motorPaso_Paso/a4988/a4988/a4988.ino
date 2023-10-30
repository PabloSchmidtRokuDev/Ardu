const int dirPin = 5;
const int stepPin = 2;

const int steps = 2000;
int stepDelay;

void setup() {
   // Marcar los pines como salida
   pinMode(dirPin, OUTPUT);
   pinMode(stepPin, OUTPUT);
  // Serial.begin(115200);
   stepDelay = 500;
}

void loop() {
   //Activar una direccion y fijar la velocidad con stepDelay
   digitalWrite(dirPin, HIGH);
   //stepDelay += 10;
   //Serial.println("velocidad: "+ String(stepDelay));

   // Giramos 200 pulsos para hacer una vuelta completa
   for (int x = 0; x < steps * 1; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(stepDelay);
   }
   delay(1000);

   //Cambiamos la direccion y aumentamos la velocidad
   digitalWrite(dirPin, LOW);

   // Giramos 400 pulsos para hacer dos vueltas completas
   for (int x = 0; x < steps * 2; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(stepDelay);
   }
   delay(1000);
}