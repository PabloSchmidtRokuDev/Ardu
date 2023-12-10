
#define RAW_BUFFER_LENGTH 180

#define MARK_EXCESS_MICROS 20  // Adapt it to your IR receiver module. 20 is recommended for the cheap VS1838 modules.


#include <IRremote.hpp>
#include "codigosIR.hpp"
#include <RGBWConverter.h>

RGBWConverter converter(240, 215, 200, true);

#define DEBUG_SERIAL Serial
int pin_R = 2;
int pin_G = 0;
int pin_B = 4;
int pin_W = 5;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
int LED_W_value = 0;

int LED_R_Brillo_value = 10;
int LED_G_Brillo_value = 10;
int LED_B_Brillo_value = 10;
int LED_W_Brillo_value = 10;

int LED_R_Current_value = 0;
int LED_G_Current_value = 0;
int LED_B_Current_value = 0;
int LED_W_Current_value = 0;

int pin_btn_Mas = 14;
int pin_btn_Menos = 16;
int pin_btn_Brillo_Blanco = 13;
int pin_btn_Brillo_Colores = 12;
int pin_btn_Color = 15;  //16

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempoLed1 = 0;
unsigned long tiempoLed2 = 0;


#define TOTAL_EFECTOS 2
uint8_t gCurrentPatternNumber = 0;

int colorSelect = 100;
int brillo_Colores = 100;
int brillo_Colores_current = 100;
int brillo_Blanco = 0;

#include "luces.hpp"
#include "colores.hpp"

void setup() {
  Serial.begin(115200);
  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_W, OUTPUT);

  pinMode(pin_btn_Mas, INPUT);
  pinMode(pin_btn_Menos, INPUT);
  pinMode(pin_btn_Brillo_Blanco, INPUT);
  pinMode(pin_btn_Brillo_Colores, INPUT);
  pinMode(pin_btn_Color, INPUT);

  analogWrite(pin_R, 0);
  analogWrite(pin_B, 0);
  analogWrite(pin_G, 0);
  analogWrite(pin_W, 0);

  cargarVecctorColores();


  colorX ccl = colorXList[random(0, 199)];
  LED_R_Brillo_value = ccl.r;
  LED_G_Brillo_value = ccl.g;
  LED_B_Brillo_value = ccl.b;
  LED_W_Brillo_value = random(10, 50);
  Serial.println("cambio.." + String(LED_R_Brillo_value));

  Serial.println("start..");
  DEBUG_SERIAL.begin(115200);

  tiempo1 = millis();

  tiempoLed1 = millis();
}

void loop() {

  int estado = setLedColor();
  tiempo2 = millis();
  if (tiempo2 >= tiempo1 + 30) {
    // Serial.print("_Mas.." + String(digitalRead(pin_btn_Mas)));
    //  Serial.print("\t_Menos.." + String(digitalRead(pin_btn_Menos)));
    //   Serial.print("\t _Color.." + String(digitalRead(pin_btn_Color)));
    //    Serial.print("\t _Brillo_Colores.." + String(digitalRead(pin_btn_Brillo_Colores)));
    //   Serial.println("\t_Brillo_Blanco.." + String(digitalRead(pin_btn_Brillo_Blanco)));



    //Serial.println("estado.."+ String(estado));
    if (estado == 2) {
      checkIR();
      // if (digitalRead(pin_btn_Mas) == HIGH) {
      //   if (digitalRead(pin_btn_Brillo_Blanco) == HIGH) brillo_Blanco++;
      //   if (digitalRead(pin_btn_Brillo_Colores) == HIGH) brillo_Colores++;
      //   if (digitalRead(pin_btn_Color) == HIGH) colorSelect++;

      //   if (brillo_Blanco >= 255) brillo_Blanco = 254;
      //   if (brillo_Colores > 100) brillo_Colores = 100;
      //   if (colorSelect > 500) colorSelect = 499;
      // }

      // if (digitalRead(pin_btn_Menos) == HIGH) {
      //   if (digitalRead(pin_btn_Brillo_Blanco) == HIGH) brillo_Blanco--;
      //   if (digitalRead(pin_btn_Brillo_Colores) == HIGH) brillo_Colores--;
      //   if (digitalRead(pin_btn_Color) == HIGH) colorSelect--;

      //   if (brillo_Blanco < 0) brillo_Blanco = 1;
      //   if (brillo_Colores < 0) brillo_Colores = 1;
      //   if (colorSelect < 0) colorSelect = 1;
      // }

      // colorX ccl = colorXList[colorSelect];
      // LED_R_Brillo_value = ccl.r;
      // LED_G_Brillo_value = ccl.g;
      // LED_B_Brillo_value = ccl.b;
      // LED_W_Brillo_value = brillo_Blanco;
    }

    tiempo1 = millis();
  }
}

void executeFunction(){
  
}

void checkIR() {
  if (IrReceiver.decode()) {  // Grab an IR code

    // Check if the buffer overflowed
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
      // Serial.println(F("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__));
      // see also https://github.com/Arduino-IRremote/Arduino-IRremote#compile-options--macros-for-this-library
    } else {
      IrReceiver.stop();
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      } else {
        if (IrReceiver.decodedIRData.decodedRawData != 0) {
          Serial.print("value:");
          Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);


          String Code = buscarCodigoEnLista(IrReceiver.decodedIRData.decodedRawData);
          if (Code != "") {
            executeFunction(Code);
          }
          
        }
      }
      IrReceiver.start();
    }
    IrReceiver.resume();  // Prepare for the next value
  }
}
