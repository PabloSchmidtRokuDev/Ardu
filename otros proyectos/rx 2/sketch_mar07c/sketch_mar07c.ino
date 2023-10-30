#include <ir_Lego_PF_BitStreamEncoder.h>
#include <boarddefs.h>
#include <IRremoteInt.h>
#include <IRremote.h>

void setup()
{
#define IR_PIN    3 //IR receiver Signal pin connect to Arduino pin D2 
 IRrecv IR(IR_PIN);  //   IRrecv object  IR get code from IR remoter
 decode_results IRresults; 
    pinMode(4, OUTPUT);

  pinMode(IR_PIN, INPUT); 
  digitalWrite(IR_PIN, HIGH);  
  IR.enableIRIn();     

  
   Serial.begin(9600);
   Serial.println("ir iniciando....");
   digitalWrite(4, true);
    delay(2000);
    digitalWrite(4, false);

     Serial.println("ir iniciando. ok");
}

void loop()
{
  if(IR.decode(&IRresults))
  {
    //if(IRresults.value

      Serial.println(IRresults.value);

    IRresults.value = 0;
    IR.resume();
  }

}
