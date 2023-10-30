/*
  Creado: Luis del Valle (ldelvalleh@programarfacil.com
  Utilización librería Steper con motor 28BYJ-48
  https://programarfacil.com
*/

// Incluímos la librería para poder utilizarla
#include <Stepper.h>

// Esto es el número de pasos por revolución
#define STEPS 3000 
// Número de pasos que queremos que de
#define NUMSTEPS 200

// Constructor, pasamos STEPS y los pines donde tengamos conectado el motor
Stepper stepper(STEPS, 2, 3, 4, 5);

void setup() {
  // Asignamos la velocidad en RPM (Revoluciones por Minuto)
  stepper.setSpeed(3);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  // Movemos el motor un número determinado de pasos
  digitalWrite(6, HIGH);
  stepper.step(NUMSTEPS);
  delay(1000);

digitalWrite(6, LOW);
digitalWrite(7, HIGH);
  stepper.step(-NUMSTEPS);
  delay(1000);

digitalWrite(6, HIGH);
digitalWrite(7, LOW);

    stepper.step(NUMSTEPS);

  delay(1000);

digitalWrite(6, HIGH);
digitalWrite(7, HIGH);
    stepper.step(NUMSTEPS);
  delay(1000);

digitalWrite(7, HIGH);
digitalWrite(6, LOW);
    stepper.step(-NUMSTEPS);
  delay(1000);
  
}
