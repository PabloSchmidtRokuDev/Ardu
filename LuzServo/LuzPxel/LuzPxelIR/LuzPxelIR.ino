#include <IRremote.h>

//IR REMOTE
#define PIN_OUT_1  2
#define PIN_OUT_2  3
#define PIN_OUT_3  4
#define PIN_OUT_4  5
#define PIN_OUT_5  6
#define PIN_OUT_6  7
#define PIN_OUT_7  8
#define PIN_OUT_8  9
#define PIN_OUT_9  10
#define PIN_OUT_10  11

#define IR_PIN  13
IRrecv IR(2);  //   IRrecv object  IR get code from IR remoter
decode_results IRresults; 

enum DN
{ 
  GO_POWER,
  GO_B_MENOS, 
  GO_B_MAS,
  GO_VOL_MAS,
  GO_VOL_MENOS,
  GO_ADELANTE,
  GO_ATRAS,
  GO_RGB,
  GO_WARM,
  GO_COOL,
  GO_MODE,
  DEF
}

  Drive_Num=DEF;
  DN Last_Drive_Num=DEF;

  bool powerOn = false;

void setup(){
  Serial.begin(9600); 
  IR.enableIRIn();

  pinMode(PIN_OUT_1, OUTPUT);
  pinMode(PIN_OUT_2, OUTPUT);
  pinMode(PIN_OUT_3, OUTPUT);
  pinMode(PIN_OUT_4, OUTPUT);
  pinMode(PIN_OUT_5, OUTPUT);
  pinMode(PIN_OUT_6, OUTPUT);
  pinMode(PIN_OUT_7, OUTPUT);
  pinMode(PIN_OUT_8, OUTPUT);
  pinMode(PIN_OUT_9 , OUTPUT);
  pinMode(PIN_OUT_10 , OUTPUT);
  
}


void loop(){
  do_IR_Tick();

  if (Drive_Num == GO_POWER) {
      digitalWrite(PIN_OUT_1, HIGH); 
      Serial.print("PIN_OUT_1");
      delay(500);
      digitalWrite(PIN_OUT_1, LOW);

  }

  if (Drive_Num == GO_B_MENOS) {

  }

  if (Drive_Num == GO_B_MAS) {
   
  }

  if (Drive_Num == GO_VOL_MAS) {
      digitalWrite(PIN_OUT_2, HIGH); 
      Serial.print("PIN_OUT_2");
      delay(500);
      digitalWrite(PIN_OUT_2, LOW);
  }

  if (Drive_Num == GO_VOL_MENOS) {
      digitalWrite(PIN_OUT_3, HIGH); 
      Serial.print("PIN_OUt_3");
      delay(500);
      digitalWrite(PIN_OUT_3, LOW);
  }

  if (Drive_Num == GO_ADELANTE) {
      digitalWrite(PIN_OUT_4, HIGH); 
      Serial.print("PIN_OUT_4");
      delay(500);
      digitalWrite(PIN_OUT_4, LOW);
  }

  if (Drive_Num == GO_ATRAS) {
      digitalWrite(PIN_OUT_5, HIGH); 
      Serial.print("PIN_OUT_5");
      delay(500);
      digitalWrite(PIN_OUT_5, LOW);
  }

  if (Drive_Num == GO_RGB) {

  }

  if (Drive_Num == GO_WARM) {

  }

  if (Drive_Num == GO_COOL) {

  }

  if (Drive_Num == GO_MODE) {

  }

  if (Drive_Num == DEF) {

  }

}

void do_IR_Tick()
{
  Drive_Num=DEF;
  bool  r = IR.decode(&IRresults);
  
  Serial.println(String(IRresults.value));
  
  if(r)
  {
    
    IR.resume();
    String str = String(IRresults.value);
    Serial.println(".-.-.-.-.-.-.-.-.-.-.-.-.-.");
    Serial.println(str);

    if (str == "16736925" || str == "none") {
      Serial.println("GO_POWER");
      Drive_Num = GO_POWER;
    }

    else if (str == "16753245" || str == "none") {
      Serial.println("GO_B_MENOS,");
      Drive_Num = GO_B_MENOS;
    }

    else if (str == "16769565" || str == "none") {
      Serial.println("GO_B_MAS");
      Drive_Num = GO_B_MAS;
    }

    else if (str == "16712445" || str == "none") {
      Serial.println("GO_VOL_MAS");
      Drive_Num = GO_VOL_MAS;
    }

    else if (str == "16750695" || str == "none") {
      Serial.println("GO_VOL_MENOS");
      Drive_Num = GO_VOL_MENOS;
    }

    else if (str == "16748655" || str == "none") {
      Serial.println("GO_ADELANTE");
      Drive_Num = GO_ADELANTE;
    }

    else if (str == "16769055" || str == "none") {
      Serial.println("GO_ATRAS");
      Drive_Num = GO_ATRAS;
    }

    else if (str == "16763445" || str == "none") {
      Serial.println("GO_RGB");
      Drive_Num = GO_RGB;
    }

    else if (str == "16724175" || str == "none") {
      Serial.println("GO_WARM");
      Drive_Num = GO_WARM;
    }

    else if (str == "16743045" || str == "none") {
      Serial.println("GO_COOL");
      Drive_Num = GO_COOL;
    }

    else if (str == "16718055" || str == "none") {
      Serial.println("GO_MODE");
      Drive_Num = GO_MODE;
    }
    else if(str != "4294967295")
    {
      Drive_Num = Last_Drive_Num;
    }
    else
    {
      Drive_Num = DEF;
        Serial.println("no reconocida: ");
        Serial.println(IRresults.value);
        Serial.println(" --> ");
        Serial.println(str);
    }
    IRresults.value = 0; 

/*


    if(str == "16712445" || str == "2747854299")
    {
      Serial.println("GO_RIG");
       Drive_Num=GO_RIGHT;
    }
    else if(str == "16769055" || str == "3238126971")
    {
      Serial.println("GO_ARRIBA");
      Drive_Num=GO_ARRIBA;
    }
    else if(str == "16748655" || str == "4039382595")
    {
      Serial.println("GO_ABAJO");
        Drive_Num=GO_ABAJO;
    }
    else if(str == "16750695" || str == "1033561079")
    {
      Serial.println("GO_LEFT");
       Drive_Num=GO_LEFT;
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

    else if(str == "16743045") //habilitar control - 
    {
        habilitado = true;
        Serial.println("habilitado ");
    }
   else if(str == "16724175" || str == "16718055") //desabilita control 
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
*/
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
