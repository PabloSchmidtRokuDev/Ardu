#include <Servo.h>

Servo myservo;
Servo myservo2;

const int servoPin = 3;
const int servo2Pin = 6;

const int distEntradaPin = 13;
const int distSalidaPin = 12;

const int sensorTopePin = 9;

// motos paso a paso
const int dirPin = 5;
const int stepPin = 2;
//const int enabledPin = 8;

const int stepsPerRevolution = 200;
const int delayMS = 150;
const int delayMS2 = 150;

const int delayMSFF = 100;
const int delayMS2FF = 100;

const int distanciaTope = 17044;
const int bloques = 1136;

int posicionActual = -1;
int BloqueActual = -1;

int servoPosActual = -1;
int servoPosActual2 = -1;

int buttonStatePressed = 1;
int direccionHome = 0;
int direccionOpuestaHome = 1;

void setup()
{
  Serial.begin(9600);
  // Declare pins as motor paso a paso
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  //pinMode(enabledPin, OUTPUT);

  myservo.attach(servoPin);
  myservo2.attach(servo2Pin);

  pinMode(distSalidaPin, OUTPUT); // salida del pulso generado por el sensor ultrasónico
  pinMode(distEntradaPin, INPUT); // entrada del pulso generado por el sensor ultrasónico

  pinMode(sensorTopePin, INPUT);

  Serial.println("ready go");

  volverAlaPosicion0();

  //Serial.println("Limpiar...");

  irAlaPosicionfinal();
}

int red;
String myString;
void loop()
{
  while (Serial.available() > 0)
  {
    myString = Serial.readString();
    String letra = myString.substring(0, 1);

    int textLen = myString.length() - 1;
    String resto = myString.substring(1, textLen);

    Serial.print("letra: ");
    Serial.println(letra);

    if (letra == "b")
    {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      moverBrazoPos(Valor);

    } else if (letra == "s") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      posicionServo(Valor);

    } else if (letra == "p") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      abrir_cerrar_Tapa(Valor);

    }else if(letra == "l"){
      Limpiar();
    }

    /*red = Serial.parseInt();
      if (Serial.read() == '\n')
      {
      Serial.print("value: ");
      Serial.println(red);
      if (red == 999)
        Limpiar();
        else if (red == 998){
          probarSerbo();
        }
      else if (red < 0)
      {
        red = red *-1;
        posicionServo(red);
      }
      else
      {
        moverBrazoPos(red);
      }
      }*/
  }


}

void volverAlaPosicion0()
{

  Serial.println("ir al punto 0");

  //moverBrazoDireccionHome(5, false);
  int buttonState = digitalRead(sensorTopePin);

  Serial.println("estado boton: ");
  Serial.println(buttonState);

  while (buttonState != buttonStatePressed)
  {
    moverBrazoDireccionHome(1, false);
    buttonState = digitalRead(sensorTopePin);
  }


  delay(500);

  posicionActual = 0;
  BloqueActual = 0;

  //moverBrazoDireccionOpuestaHome(distanciaTope, true);
  BloqueActual = 15;

  servoPosActual = 90;
  servoPosActual2 = 108;
  
  myservo.write(servoPosActual);
  myservo2.write(servoPosActual2);

  Serial.println("punto 0 completo");
}


void irAlaPosicionfinal()
{


  int acum = 0;

  while (acum != distanciaTope)
  {
    acum = acum + 1;
    moverBrazoDireccionOpuestaHome(1, true);

    if (acum == bloques * 4)
    {
      posicionServo(10);
    }
  }

  Serial.print("totol de pasos: ");
  Serial.println(distanciaTope);

  posicionServo(5);
}


void probarSerbo()
{
  // 1355 / 4 = 338
  //-----100- 45ª --100---90ª -- 600 ----- 130ª --- 100--- 180ª ---100 -130ª

  //  -1: inicial 0ª
  // - 2: baja  45ª
  // - 3: posicion Rastrillo 90ª
  // - 4: 130ª
  // - 5: 180ª

  Serial.println("Limpieza iniciada");


  moverBrazoPos(7);


  posicionServo(1);
  Serial.println("servo pos 1");
  delay(2000);

  posicionServo(2);
  Serial.println("servo pos 2");
  delay(2000);


  posicionServo(3);
  Serial.println("servo pos 3");
  delay(2000);


  posicionServo(4);
  Serial.println("servo pos 4");
  delay(2000);


  posicionServo(5);
  Serial.println("servo pos 5");
  delay(2000);


  posicionServo(1);
  moverBrazoPos(15);
  posicionServo(2);
  Serial.println("servo pos finalizado");
}

void Limpiar()
{
  // 1355 / 4 = 338
  //-----100- 45ª --100---90ª -- 600 ----- 130ª --- 100--- 180ª ---100 -130ª

  //  -1: inicial 0ª
  // - 2: baja  45ª
  // - 3: posicion Rastrillo 90ª
  // - 4: 130ª
  // - 5: 180ª

  Serial.println("Limpieza iniciada");

  posicionServo(5);
  abrir_cerrar_Tapa(5);
  moverBrazoPos(15);

  Serial.println("Limpieza paso 1");
  posicionServo(10);
  moverBrazoPos(3);
  Serial.println("Limpieza paso 2");
  posicionServo(8);
  moverBrazoPos(1);

  Serial.println("Limpieza paso 3");
  posicionServo(7);
  moverBrazoPos(10);

  Serial.println("Limpieza paso 4");
  posicionServo(4);
  moverBrazoPos(12);
  
  Serial.println("Limpieza paso 5");
  posicionServo(3);
  abrir_cerrar_Tapa(7);

 delay(500);
 
  Serial.println("Limpieza paso 6");

  posicionServo(2);
  abrir_cerrar_Tapa(6);
  moverBrazoPos(11);

  Serial.println("Limpieza paso 7");
  posicionServo(1);
  abrir_cerrar_Tapa(5);
  posicionServo(0);
  moverBrazoPos(6);


  Serial.println("Limpieza paso 8");
  posicionServo(1);
  abrir_cerrar_Tapa(10);
  moverBrazoPos(15);
  
   Serial.println("Limpieza paso 9");
  posicionServo(4);
  
  delay(2000);

  Serial.println("Limpieza paso 10");
  
  posicionServo(1);
  moverBrazoPos(6);
  
  Serial.println("Limpieza paso 11");
  
 abrir_cerrar_Tapa(5);
 
  Serial.println("Limpieza paso 12");
  
  moverBrazoPos(10);

  Serial.println("Limpieza paso 13");
  posicionServo(3);
  moverBrazoPos(12);

  Serial.println("Limpieza paso 14");
  posicionServo(4);
  moverBrazoPos(14);
  
  Serial.println("Limpieza paso 15");
  posicionServo(5);
  moverBrazoPos(15);


  Serial.println("Limpieza completa");
}

void moverBrazoPos(int pos)
{
  // 1355 / 4 = 338
  //-----100- 45ª --100---90ª -- 600 ----- 130ª --- 100--- 180ª ---100 -130ª

  if (BloqueActual == pos || pos > 15 || pos < 0) {
    return 0;
  }

  int moverBloques = 0;
  int moverpasos = 0;
  if (BloqueActual > pos)
  {
    moverBloques = BloqueActual - pos;
    moverpasos = bloques * moverBloques;
/*
    Serial.print("BloqueActual:  ");
    Serial.print(BloqueActual);
    Serial.print(" - ir al bloque:  ");
    Serial.print(pos);
    Serial.print(" - mover ");
    Serial.print(moverBloques);
    Serial.print(" Bloques direccion al home -pasos totales:  ");
    Serial.println(moverpasos);*/

    moverBrazoDireccionHome(moverpasos, false);
    BloqueActual = pos;
  }
  else
  {
    moverBloques = pos - BloqueActual;
    moverpasos = bloques * moverBloques;
/*
    Serial.print("BloqueActual:  ");
    Serial.print(BloqueActual);
    Serial.print(" - ir al bloque:  ");
    Serial.print(pos);
    Serial.print(" - mover ");
    Serial.print(moverBloques);
    Serial.print(" Bloques direccion OPUESTA al home -pasos totales:  ");
    Serial.println(moverpasos);
*/
    moverBrazoDireccionOpuestaHome(moverpasos, false);
    BloqueActual = pos;
  }
}

void moverBrazoDireccionHome(long pasos, bool SR)
{
  int dms1 = delayMS;
  int dms2 = delayMS2;

  if (SR == true) {
    dms1 = delayMSFF;
    dms2 = delayMS2FF;
  }

  //digitalWrite(enabledPin, LOW);
  digitalWrite(dirPin, direccionHome);

  for (long x = 0; x < pasos; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(dms1);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(dms2);
    posicionActual = posicionActual - 1;
  }
  //digitalWrite(enabledPin, HIGH);
}

void moverBrazoDireccionOpuestaHome(int pasos, bool SR)
{
  int dms1 = delayMS;
  int dms2 = delayMS2;

  if (SR == true) {
    dms1 = delayMSFF;
    dms2 = delayMS2FF;
  }
  //digitalWrite(enabledPin, LOW);

  digitalWrite(dirPin, direccionOpuestaHome);
  for (int x = 0; x < pasos; x++)
  {

    digitalWrite(stepPin, HIGH);
    delayMicroseconds(dms1);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(dms2);

    posicionActual = posicionActual + 1;
  }

  //digitalWrite(enabledPin, HIGH);
  // Serial.print("posicionActual:  ");
  // Serial.println(posicionActual);
}

// pos= 0: inicial - 1: baja  45ª - 2: posicion Rastrillo 90ª
// 3: 130ª - 4: 180ª
void posicionServo(int pos)
{
  int posX = 18 * pos;

  //Serial.print("servo ir a la posicion  ");
  //Serial.println(posX);
  moverServoPos(posX);
}

void moverServoPos(int pos) {

  while (servoPosActual != pos)
  {
    if (servoPosActual < pos) {
      servoPosActual++;
    }
    else {
      servoPosActual--;
    }
    
    myservo.write(servoPosActual);
    delayMicroseconds(15000);

  }
}

void abrir_cerrar_Tapa(int posicion) {

  int posX = 18 * posicion;

  //Serial.print("servo 2 ir a la posicion  ");
  //Serial.println(posX);
  moverServo2Pos(posX);
}

void moverServo2Pos(int pos) {

  while (servoPosActual2 != pos)
  {
    if (servoPosActual2 < pos) {
      servoPosActual2++;
    }
    else {
      servoPosActual2--;
    }
    
    myservo2.write(servoPosActual2);
    delayMicroseconds(15000);

  }
}

long leerDistancia()
{
  long dis;
  long tiem;

  digitalWrite(distSalidaPin, LOW); // recibimiento del pulso.
  delayMicroseconds(5);
  digitalWrite(distSalidaPin, HIGH); // envió del pulso.
  delayMicroseconds(10);

  tiem = pulseIn(distEntradaPin, HIGH); // fórmula para medir el pulso entrante.

  dis = long(0.017 * tiem); // fórmula para calcular la distancia del sensor ultrasónico.

  return dis;
}
