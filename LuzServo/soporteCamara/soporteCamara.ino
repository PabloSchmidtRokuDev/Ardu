
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
#define IR_PIN  10
#define LED_PIN  9
#define PHOTO_PIN  8

#define ANALOG1_PIN  0
#define ANALOG2_PIN  1

#define SERVO_PIN1_DEF_POS 90
#define SERVO_PIN2_DEF_POS  90

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
  POWER_ON,
  POWER_OFF,
  DEF
}

  Drive_Num=DEF;
  DN Last_Drive_Num=DEF;

 IRrecv IR(IR_PIN);  //   IRrecv object  IR get code from IR remoter
 decode_results IRresults; 

int distance1 = SERVO_PIN1_DEF_POS;
Servo servo1;

int distance2 = SERVO_PIN2_DEF_POS;
Servo servo2;

int incrementos = 4;
bool habilitado = false;

void setup() {
  Serial.begin(9600);

  pinMode(PHOTO_PIN, OUTPUT); 
  pinMode(LED_PIN, OUTPUT); 

  IR.enableIRIn();
  
  servo1.attach(SERVO_PIN1); 
  servo1.write(distance1);

  servo2.attach(SERVO_PIN2); 
  servo2.write(distance2); 

  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  /*
  tiempoInicial = millis();

  tiempoFlash = millis();
  tiempoInactividad = millis();

 
     Serial.print("tiempoInactividad ");
    Serial.println(tiempoInactividad);
      
    
     Serial.print("tiempoParaInactividad ");
    Serial.println(tiempoParaInactividad);
     */
     Serial.println(".-.-.-.-start!!.-.-.-.-.-.-.-.-.-.");
}

int lectura = 0;
unsigned long suma;
unsigned long dif;

void loop() {
  tiempoContador = millis();

  if ((tiempoContador > tiemploParaFlash + tiempoFlash) && Drive_Num != VERIFICACION_ACCION){
      tiempoFlash = tiempoContador + tiemploParaFlash;
      digitalWrite(LED_PIN, HIGH);
      delay(10);
      digitalWrite(LED_PIN, LOW);
  }

  do_IR_Tick();

  if (habilitado == true){
    if (Drive_Num != DEF && Drive_Num != VERIFICACION_ACCION) {
        //tiempoInactividad = tiempoContador + tiempoParaInactividad;
      
        if ((Drive_Num == GO_RIGHT) || (Drive_Num == GO_LEFT) || (Drive_Num == GO_ARRIBA) || (Drive_Num == GO_ABAJO))
        {
          Last_Drive_Num = Drive_Num;
  
          for (int i = 0; i <= incrementos; i++) 
          {
            if (Drive_Num == GO_RIGHT) distance1 += 1;
            if (Drive_Num == GO_LEFT) distance1 -= 1;
            if (Drive_Num == GO_ARRIBA) distance2 += 1;
            if (Drive_Num == GO_ABAJO) distance2 -= 1;
            setServoPosition();
            delay(25);
          }
        }
  
  
        if (Drive_Num==AGREGAR_DISTANCIA){
          //Serial.println("suma velocidad: ");
          modificarDistancia(2);
        }
        
        if (Drive_Num==QUITAR_DISTANCIA){
          //Serial.println("resta velocidad: ");
          modificarDistancia(-2);
        }
  
  
        if (Drive_Num==RAFAGA_FOTOS){
          //Serial.println("Tomar varias Foto ");
          tomarVariasFotos();
          Drive_Num=DEF;
          Last_Drive_Num=DEF;
        }
  
        if (Drive_Num==TOMAR_1_FOTO){
          //Serial.println("Tomar 1 Foto ");
          tomarUnaFoto();
          Drive_Num=DEF;
          Last_Drive_Num=DEF;
        }
  
      Drive_Num=DEF;
    }
  }
}


void do_IR_Tick()
{
  if(IR.decode(&IRresults))
  {
    
    IR.resume();
    String str = String(IRresults.value);
    Serial.println(".-.-.-.-.-.-.-.-.-.-.-.-.-.");
    Serial.println(str);

    if(str == "16769055" || str == "3238126971")
    {
      Serial.println("GO_LEFT");
       Drive_Num=GO_LEFT;
      
    }
    else if(str == "16750695" || str == "1033561079")
    {
      Serial.println("GO_ARRIBA");
      Drive_Num=GO_ARRIBA;
    }
    else if(str == "16712445" || str == "2747854299")
    {
      Serial.println("GO_ABAJO");
        Drive_Num=GO_ABAJO;
    }
    else if(str == "16748655" || str == "4039382595")
    {
      Serial.println("GO_RIG");
       Drive_Num=GO_RIGHT;
    }

    
    else if(str == "4294967295")
    {
       Drive_Num = Last_Drive_Num;
    }
    else if(str == "16769565" || str == "1618364903")
    {
      Drive_Num=AGREGAR_DISTANCIA;
      Serial.println("suma velocidad: ");
    }
    else if(str == "16753245" || str == "2928683316")
    {
      Drive_Num=QUITAR_DISTANCIA;
      Serial.println("resta velocidad: ");
    }

    
    else if(str == "16724175") //habilitar control - warm
    {
        habilitado = true;
        Serial.println("habilitado ");
     
    }
   else if(str == "16743045" || str == "16718055") //desabilita control -cool or mode
    {
        habilitado = false;
        Serial.println("desabilitado ");
     
    }
    
    else if(str == "16754775") {
      Drive_Num = TOMAR_1_FOTO;
        Serial.println("Tomar 1 Foto "); 
    }
    
    else if(str == "16763445")  {
        Drive_Num=RAFAGA_FOTOS;
      Serial.println("Tomar varias Foto ");

    } else {
        Serial.println("no reconocida: ");
        Serial.println(IRresults.value);
        Serial.println(" --> ");
        Serial.println(str);
    }

    Serial.println("resume..: ");
    //IRresults.value = 0; 

  }
}

void modificarDistancia(int inc)
{     Serial.println("funcion modificarDistancia ");
      incrementos = incrementos + inc;
      if (incrementos <= 0){
        incrementos = 2;
      }
      
      Drive_Num=DEF;
      Last_Drive_Num=DEF;
      
      Serial.println(incrementos);

      for(int i=0; i<=incrementos; i++){
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
      }

}

void tomarUnaFoto(){
  Serial.println("funcion tomarUnaFoto ");
      digitalWrite(LED_PIN, HIGH);
      delay(200);
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

void setServoPosition(){
  Serial.println("servo 1: ");
  Serial.println(distance1);
  Serial.println(" -  servo 2: ");
  Serial.println(distance2);

  
  servo1.write(distance1); 
  servo2.write(distance2); 
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
