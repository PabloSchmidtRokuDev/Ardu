#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
#include <SoftwareSerial.h>;

MCUFRIEND_kbv tft;
// constants won't change. They're used here to set pin numbers:
const int button1Pin = 9;     // the number of the pushbutton pin
const int button2Pin = 11;     // the number of the pushbutton pin
const int accion1Pin =  31;      // the number of the LED pin
const int accion2Pin =  33;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status



#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410


SoftwareSerial camSerial(51, 53);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  uint16_t ID = tft.readID();

    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(0);
    
  tft.fillScreen(BLACK);
  addText("mega - iniciando...");
  
  pinMode(accion1Pin, OUTPUT);
  pinMode(accion2Pin, OUTPUT);

  addText("mega - escuchando....");
}

String data;
bool TransmisionEntrante = false;
bool TransmisionComplata = false;
void loop() {


  while (Serial1.available() && TransmisionComplata == false) {
    TransmisionComplata = false;
    char CharEntrada = Serial1.read(); //Leer un byte del puerto serial

    data += CharEntrada;  //Agregar el char anterior al string

    TransmisionEntrante = true;
    
    if(CharEntrada == '\n'){
      TransmisionComplata = true;
    }
  }
  
    if (TransmisionComplata == true)
    {
        data.trim();
        addText("trasmision Completa.");
        addText(data);
        
        if (data == "#E") {
        addText("Ejecutando accion 1");
        digitalWrite(accion1Pin, HIGH);
        delay(5000);
        addText("accion 1 terminado.");
        digitalWrite(accion1Pin, LOW);
      }
    
       if (data == "#H")  {
        addText("ejecutando accion 2");
        digitalWrite(accion2Pin, HIGH);
        delay(5000);
        addText("accion 2 terminado.");
        digitalWrite(accion2Pin, LOW);
      }

      data = "";
      TransmisionComplata = false;
  }

}


int yActual = 17;

void addText(String msg){
  Serial.println(msg);
  int x = 20;
   
  int sz = 1;
  
  showmsgXY(x, yActual, sz, NULL, msg.c_str());
  yActual = yActual + 17;

  if (yActual > 238){
    yActual = 17;
    tft.fillScreen(RED);
  }
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y + 1, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
    delay(1000);
}
