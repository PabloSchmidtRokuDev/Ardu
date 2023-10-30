
#include <IRremote.h>
#include "LowPower.h"
#include <Servo.h>

unsigned long  tiemploParaFlash = 10000;
unsigned long  tiempoParaInactividad = 10000;//*5; //1 seg * 60 seg * 5 = 5 min

unsigned long tiempoFlash = 0;
unsigned long tiempoInactividad = 0;
unsigned long tiempoContador = 0;
unsigned long tiempoInicial = 0;

#define SERVO_PIN1  11
#define SERVO_PIN2  12
#define IR_PIN  9
#define LED_PIN  8
#define PHOTO_PIN  10

#define ANALOG1_PIN  0
#define ANALOG2_PIN  1

#define SERVO_PIN1_DEF_POS  0
#define SERVO_PIN2_DEF_POS  60

enum DN
{
  GO_ARRIBA, //go forward
  GO_LEFT, //left turn
  GO_RIGHT,//right turn
  GO_ABAJO,//backward
  STOP_STOP,
  VERIFICACION_ACCION,
  RAFAGA_FOTOS,
  TOMAR_1_FOTO,
  AGREGAR_DISTANCIA,
  QUITAR_DISTANCIA,
  POWER_PRESSED,
  DEF
}

Drive_Num = DEF;
DN Last_Drive_Num = DEF;
int luzEstado = 0;

IRrecv IR(IR_PIN);  //   IRrecv object  IR get code from IR remoter
decode_results IRresults;

int distance1 = SERVO_PIN1_DEF_POS;
int servo1PosActual = SERVO_PIN1_DEF_POS;
Servo servo1;

int distance2 = SERVO_PIN2_DEF_POS;
int servo2PosActual = SERVO_PIN1_DEF_POS;
Servo servo2;

int incrementos = 4;
boolean habilitado = false;

void setup() {
  Serial.begin(9600);

  pinMode(PHOTO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  IR.enableIRIn();

  servo1.attach(SERVO_PIN1);
  servo1.write(SERVO_PIN1_DEF_POS);

  servo2.attach(SERVO_PIN2);
  servo2.write(SERVO_PIN2_DEF_POS);

  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);

    
}

int lectura = 0;
unsigned long suma;
unsigned long dif;

void loop() {
  tiempoContador = millis();

  do_IR_Tick();
  setServoPosition();

  if (habilitado == true && Drive_Num != DEF ) {

      //tiempoInactividad = tiempoContador + tiempoParaInactividad;

      if (Drive_Num == POWER_PRESSED) {
          digitalWrite(PHOTO_PIN, LOW);
          luzEstado = 0;

          distance1 = SERVO_PIN1_DEF_POS;
          servo1PosActual = SERVO_PIN1_DEF_POS;
          distance2 = SERVO_PIN2_DEF_POS;
          servo2PosActual = SERVO_PIN1_DEF_POS;
          
          servo1.write(distance1);
          servo2.write(distance2);
        
      } else  
      if ((Drive_Num == GO_RIGHT) || (Drive_Num == GO_LEFT) || (Drive_Num == GO_ARRIBA) || (Drive_Num == GO_ABAJO))
      {
        Last_Drive_Num = Drive_Num;
        if (Drive_Num == GO_RIGHT) distance1 += incrementos;
        if (Drive_Num == GO_LEFT) distance1 -= incrementos;
        if (Drive_Num == GO_ARRIBA) distance2 += incrementos;
        if (Drive_Num == GO_ABAJO) distance2 -= incrementos;


        if (distance1 > 180) distance1 = 180;
        if (distance1 < 0) distance1 = 0;

        if (distance2 > 180) distance2 = 180;
        if (distance2 < 0) distance2 = 0;
  
      }

      if (Drive_Num == AGREGAR_DISTANCIA) {
        //Serial.println("suma velocidad: ");
        modificarDistancia(2);
      }

      if (Drive_Num == QUITAR_DISTANCIA) {
        //Serial.println("resta velocidad: ");
        modificarDistancia(-2);
      }

      if (Drive_Num == RAFAGA_FOTOS) {
        //Serial.println("Tomar varias Foto ");
        tomarVariasFotos();
        Drive_Num = DEF;
        Last_Drive_Num = DEF;
      }

      if (Drive_Num == TOMAR_1_FOTO) {
        //Serial.println("Tomar 1 Foto ");
        encenderLuz();
        Drive_Num = DEF;
        Last_Drive_Num = DEF;
      }

      Drive_Num = DEF;
    }
}


void do_IR_Tick()
{
  if (IR.decode(&IRresults))
  {

    IR.resume();
    String str = String(IRresults.value);
    //Serial.println(".-.-.-.-.-.-.-.-.-.-.-.-.-.");
    //Serial.println(str);

    if (str == "16712445" || str == "2747854299")
    {
      Serial.println("GO_RIG");
      Drive_Num = GO_RIGHT;
    }
    else if (str == "16769055" || str == "3238126971")
    {
      Serial.println("GO_ARRIBA");
      Drive_Num = GO_ARRIBA;
    }
    else if (str == "16748655" || str == "4039382595")
    {
      Serial.println("GO_ABAJO");
      Drive_Num = GO_ABAJO;
    }
    else if (str == "16750695" || str == "1033561079")
    {
      Serial.println("GO_LEFT");
      Drive_Num = GO_LEFT;
    }

    else if (str == "4294967295")
    {
      Drive_Num = Last_Drive_Num;
    }
    else if (str == "16769565" || str == "1618364903")
    {
      Drive_Num = AGREGAR_DISTANCIA;
      Serial.println("suma velocidad: ");
    }
    else if (str == "16753245" || str == "2928683316")
    {
      Drive_Num = QUITAR_DISTANCIA;
      Serial.println("resta velocidad: ");
    }

    else if (str == "16736925")
    {
      Drive_Num = POWER_PRESSED;
      Serial.println("Power ");
    }


    //16743045 cool
    //16724175 warm
    //16718055 mode

    else if (str == "16743045") //habilitar control -
    {
      habilitado = true;
      Serial.println("habilitado ");
    }
    else if (str == "16724175" || str == "16718055") //desabilita control
    {
      habilitado = false;
      Serial.println("desabilitado ");
    }


    else if (str == "16754775") {
      Drive_Num = TOMAR_1_FOTO;
      Serial.println("Tomar 1 Foto ");

    }
    else if (str == "16763445")  {
      Drive_Num = RAFAGA_FOTOS;
      Serial.println("Tomar varias Foto ");

    } else {
      /*Serial.println("no reconocida: ");
      Serial.println(IRresults.value);
      Serial.println(" --> ");
      Serial.println(str);*/
    }

    //Serial.println("resume..: ");
    //IRresults.value = 0;

  }
}

void modificarDistancia(int inc)
{ Serial.println("funcion modificarDistancia ");
  incrementos = incrementos + inc;
  if (incrementos <= 0) {
    incrementos = 2;
  }

  Drive_Num = DEF;
  Last_Drive_Num = DEF;

  Serial.println(incrementos);

  for (int i = 0; i <= incrementos; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(20);
    digitalWrite(LED_PIN, LOW);
  }

}

void encenderLuz() {
  Serial.println("funcion tomarUnaFoto ");
  if (luzEstado == 0)
  {
    digitalWrite(PHOTO_PIN, HIGH);
    luzEstado = 1;
  } else
  {
    digitalWrite(PHOTO_PIN, LOW);
    luzEstado = 0;
  }
}


void tomarUnaFoto() {
  Serial.println("funcion tomarUnaFoto ");
  digitalWrite(LED_PIN, HIGH);
  delay(20);
  digitalWrite(LED_PIN, LOW);


  delay(500);
  digitalWrite(PHOTO_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(PHOTO_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void tomarVariasFotos() {
  Serial.println("functin tomarVariasFotos ");

  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(100);

  digitalWrite(PHOTO_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(PHOTO_PIN, LOW);
  digitalWrite(LED_PIN, LOW);


  delay(500);
  digitalWrite(PHOTO_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(PHOTO_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  delay(500);
  digitalWrite(PHOTO_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(PHOTO_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  delay(500);
  digitalWrite(PHOTO_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(PHOTO_PIN, LOW);
  digitalWrite(LED_PIN, LOW);


  delay(500);
  digitalWrite(PHOTO_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(PHOTO_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

}

void setServoPosition() {

  if (distance1 != servo1PosActual || distance2 != servo2PosActual) {
  /*Serial.print("servo 1: ");
  Serial.print(distance1);
  Serial.print("  - pos actual: ");
  Serial.print(servo1PosActual);
  
  Serial.print(" ---  servo 2: ");
  Serial.print(distance2);
  Serial.print(" - pos actual: ");
  Serial.println(servo2PosActual);
      */
    if (distance1 != servo1PosActual && distance1 > servo1PosActual) servo1PosActual++;
    if (distance1 != servo1PosActual && distance1 < servo1PosActual) servo1PosActual--;

    if (distance2 != servo2PosActual && distance2 > servo2PosActual) servo2PosActual++;
    if (distance2 != servo2PosActual && distance2 < servo2PosActual) servo2PosActual--;

    servo1.write(servo1PosActual);
    servo2.write(servo2PosActual);
  }
}

void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.println("Could not decode message");
  }
  else {
    if (results->decode_type == NEC) {
      Serial.println("Decoded NEC: ");
    }
    else if (results->decode_type == SONY) {
      Serial.println("Decoded SONY: ");
    }
    else if (results->decode_type == RC5) {
      Serial.println("Decoded RC5: ");
    }
    else if (results->decode_type == RC6) {
      Serial.println("Decoded RC6: ");
    }

    Serial.println(results->value, HEX);
    Serial.println(" (");
    Serial.println(results->bits, DEC);
    Serial.println(" bits)");
  }
  Serial.println("Raw (");
  Serial.println(count, DEC);
  Serial.println("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.println(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.println(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.println(" ");
  }
  Serial.println("");
}
