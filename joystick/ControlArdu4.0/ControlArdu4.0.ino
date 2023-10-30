#include <Arduino.h>


#include <WiFi.h>
#include <WebSocketsServer.h>

#define DEBUG_SERIAL Serial


#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

#include <SPI.h>
#include <TFT_eSPI.h>  // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite class needs to be invoked


int xpos = 0;  // Half the screen width
int ypos = 50;

String pantallaS = "";

int poten_Rigth = 36;       // Analog Pin  X

int JoyStick_X = 33;       // Analog Pin  X
int JoyStick_Y = 32;       // // Analog Pin  Y
int JoyStick_button = 17;  // IO Pin

int JoyStick2_X = 25;       // Analog Pin  X
int JoyStick2_Y = 26;       // // Analog Pin  Y
int JoyStick2_button = 13;  // IO Pin

int main_button1 = 0;   // IO Pin
int main_button2 = 35;  // IO Pin

int aux_button1 = 2;  // IO Pin
int aux_button2 = 15;  // IO Pin

int start_down = 1200;
int start_up = 2100;

#define TFT_BROWN 0x38E0

int periodo = 1000;
unsigned long goToSleepIn = 0;

//#define BUTTON_PIN_BITMASK 0x800000000  // 2^33 in hex

RTC_DATA_ATTR int bootCount = 0;

#include "pantalla.hpp"
#include "conexiones.hpp"
#include "colores.hpp"
#include "fsnPantalla.hpp"


void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

long getSeconds() {
  return millis() / 1000;
}

void setup() {

  Serial.begin(115200);
  DEBUG_SERIAL.begin(115200);

  pinMode(JoyStick_X, INPUT);
  pinMode(JoyStick_Y, INPUT);

  pinMode(JoyStick2_X, INPUT);
  pinMode(JoyStick2_Y, INPUT);

  pinMode(poten_Rigth, INPUT);

  pinMode(JoyStick_button, INPUT_PULLUP);
  pinMode(JoyStick2_button, INPUT_PULLUP);
  
  pinMode(main_button1, INPUT_PULLUP);
  pinMode(main_button2, INPUT_PULLUP);

  pinMode(aux_button1, INPUT_PULLUP);
  pinMode(aux_button2, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(3);
  spr.setColorDepth(16);  // 16 bit colour needed to show anti-aliased fonts

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set the font colour and the background colour
  tft.setTextDatum(TC_DATUM);              // Top Centre datum

  xpos = tft.width() / 2 - 60;  // Half the screen width
  ypos = 10;

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Small font
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  spr.loadFont(AA_FONT_SMALL);              // Must load the font first into the sprite class
  spr.setTextColor(TFT_YELLOW, TFT_BLACK);  // Set the sprite font colour and the background colour

  iniciarWifiServer();
  crearMenu();
}


int contadorGoToSleep = 0;

void loop() {
  // }
  // void xxxx(){
  webSocket.loop();

  if (opcionSelect == 0) {  //menu principal
    String Data = readSensores();

    if (Data != "") {
      accionData(Data);
    }

  } else if (opcionSelect == 1) {  // luz 01

    String Data = readSensores2();

    if (Data != "") {
      if (Data != "") {
        Data = accionDataLuz_01(Data);
        SendDataLuz_01(Data);
      } else {
        delay(100);
      }
    }

  } else if (opcionSelect == 2) {  // soporte
  String Data = readSensores2();

    if (Data != "") {
      if (Data != "") {
        Data = accionDataLuz_01(Data);
        SendDataSoporteMovil(Data);
      } else {
        delay(100);
      }
    }
    // String Data = readSensores();
    // if (Data != "") {
    //   Data = accionDataServo(Data);
    //   if (Data != "") {
    //     updatePantallaSoporte("sendData", Data);
    //     SendDataSopote(Data);
    //   } else {
    //     delay(100);
    //   }
    // }

  } else if (opcionSelect == 3 || opcionSelect == 4) {  // soporte
    String Data = readSensores();
    if (Data != "") {
      Data = accionDataServoBrazo(Data);
      if (Data != "") {
        updatePantallaBrazo01("sendData", Data);
        SendDataBrazo01(Data);
      } else {
        delay(100);
      }
    }
  }

  
}

String accionDataLuz_01(String dataAvailable) {
  if (dataAvailable != "" && dataAvailable.indexOf("|") != -1) {
    String cm = dataAvailable.substring(0, dataAvailable.indexOf("|"));
    Serial.print("1 -cm: " + cm);

    String obj = cm.substring(0, cm.indexOf(":"));
    Serial.print("\t obj: " + obj);
    cm = cm.substring(cm.indexOf(":") + 1);
    Serial.println(" \t 3- cm final: " + cm);

    if (obj == "B1" && cm == "1") {
      crearMenu();
      return "";
    }
  }
  return dataAvailable;
}

String accionDataServoBrazo(String dataAvailable) {
  if (dataAvailable != "" && dataAvailable.indexOf("|") != -1) {
    String cm = dataAvailable.substring(0, dataAvailable.indexOf("|"));
    Serial.print("1 -cm: " + cm);

    dataAvailable = dataAvailable.substring(dataAvailable.indexOf("|") + 1);
    Serial.print(" \t 2- new  dataAvailable: " + dataAvailable);

    String obj = cm.substring(0, cm.indexOf(":"));
    Serial.print("\t obj: " + obj);
    cm = cm.substring(cm.indexOf(":") + 1);
    Serial.println(" \t 3- cm final: " + cm);

    if (obj == "TB" && cm == "0") {
      if (subOpcionSelect == 1) {
        updatePantallaBrazo01("Ejes3y4", "");
      } else if (subOpcionSelect == 2) {
        //   updatePantallaBrazo01("CR", "");
        // } else if (subOpcionSelect == 3) {
        //   updatePantallaBrazo01("CG", "");
        // } else if (subOpcionSelect == 4) {
        //   updatePantallaBrazo01("CB", "");
        // } else if (subOpcionSelect == 5) {
        //   updatePantallaBrazo01("CW", "");
        // } else if (subOpcionSelect == 6) {
        //   updatePantallaBrazo01("CY", "");
        // } else if (subOpcionSelect == 7) {
        updatePantallaBrazo01("Ejes1y2", "");
      }

    } else if (obj == "B1" && cm == "1") {

      if (subOpcionSelect == 10) {
        selectOptionMenu(opcionSelect);
         return "";
      } 

     
      crearMenu();

      return "";

    } else if (obj == "B2" && cm == "1") {
       if (subOpcionSelect == 10) {
        selectOptionMenu(opcionSelect);
      } 

      return "";

    }  else if (obj == "BJ" && cm == "0") {
        crearPantallaColores();
        subOpcionSelect = 10;
        return "";
      

    } else if (obj == "XA" || obj == "XB" || obj == "YB" || obj == "YA") {
      if (subOpcionSelect == 1) {
        if (obj == "XA") return "S1:A" + cm + "|";
        if (obj == "XB") return "S1:B" + cm + "|";
        if (obj == "YB") return "S2:A" + cm + "|";
        if (obj == "YA") return "S2:B" + cm + "|";

      } else if (subOpcionSelect == 2) {
        if (obj == "XA") return "S3:A" + cm + "|";
        if (obj == "XB") return "S3:B" + cm + "|";
        if (obj == "YB") return "S4:A" + cm + "|";
        if (obj == "YA") return "S4:B" + cm + "|";

      } else if (subOpcionSelect == 10) {
        colorX colorXsel;

        if (obj == "YA" && cm == "0") colorXsel = getColores(dir_rigth);
        if (obj == "YB" && cm == "0") colorXsel = getColores(dir_left);
        if (obj == "XB" && cm == "0") colorXsel = getColores(dir_down);
        if (obj == "XA" && cm == "0") colorXsel = getColores(dir_up);

        return "COLOR:" + String(colorXsel.r) + ";" + String(colorXsel.g) + ";" + String(colorXsel.b)+ ";" + String(colorXsel.nombre) + "|";
      }
    } if(obj == "TB2"){
      return "TB2:" + cm + "|";
    }
  }

  return "";
}

String accionDataServo(String dataAvailable) {
  if (dataAvailable != "" && dataAvailable.indexOf("|") != -1) {
    String cm = dataAvailable.substring(0, dataAvailable.indexOf("|"));
    Serial.print("1 -cm: " + cm);

    dataAvailable = dataAvailable.substring(dataAvailable.indexOf("|") + 1);
    Serial.print(" \t 2- new  dataAvailable: " + dataAvailable);

    String obj = cm.substring(0, cm.indexOf(":"));
    Serial.print("\t obj: " + obj);
    cm = cm.substring(cm.indexOf(":") + 1);
    Serial.println(" \t 3- cm final: " + cm);


    if (obj == "TB" && cm == "0") {
      if (subOpcionSelect == 1) {
        updatePantallaSoporte("Ejes3y4", "");
      } else if (subOpcionSelect == 2) {
        updatePantallaSoporte("Ejes5y6", "");
      } else if (subOpcionSelect == 3) {
        updatePantallaSoporte("Ejes1y2", "");
      }
    } else if (obj == "B1" && cm == "1") {
      crearMenu();
      return "";
    }

    if (obj == "XA" || obj == "XB" || obj == "YB" || obj == "YA") {
      if (subOpcionSelect == 1) {
        if (obj == "XA") return "S1:A" + cm + "|";
        if (obj == "XB") return "S1:B" + cm + "|";
        if (obj == "YB") return "S2:A" + cm + "|";
        if (obj == "YA") return "S2:B" + cm + "|";

      } else if (subOpcionSelect == 2) {
        if (obj == "XA") return "S3:A" + cm + "|";
        if (obj == "XB") return "S3:B" + cm + "|";
        if (obj == "YB") return "S4:A" + cm + "|";
        if (obj == "YA") return "S4:B" + cm + "|";
      } else if (subOpcionSelect == 3) {
        if (obj == "XA") return "S5:A" + cm + "|";
        if (obj == "XB") return "S5:B" + cm + "|";
        if (obj == "YB") return "S6:A" + cm + "|";
        if (obj == "YA") return "S6:B" + cm + "|";
      }
    }
  }

  return "";
}

void accionData(String dataAvailable) {
  if (dataAvailable != "" && dataAvailable.indexOf("|") != -1) {
    String cm = dataAvailable.substring(0, dataAvailable.indexOf("|"));
    //Serial.print("1 -cm: " + cm);

    dataAvailable = dataAvailable.substring(dataAvailable.indexOf("|") + 1);
    // Serial.print(" \t 2- new  dataAvailable: " + dataAvailable);

    String obj = cm.substring(0, cm.indexOf(":"));
    //Serial.print("\t obj: " + obj);
    cm = cm.substring(cm.indexOf(":") + 1);
    // Serial.println(" \t 3- cm final: " + cm);

    executeInstruccion(obj, cm);
  }
}

int x, y, buttonJoystick, button1, button2, touchEstado, touchEstado2;
int xp = 0;
int yp = 0;
int buttonJoystickP = 0;
int button1P = 1;
int button2P = 1;
int touchEstadoP = 0;
int touchEstadoP2 = 0;

int x2, y2, buttonJoystick2, poten;

String readSensores() {
  x = analogRead(JoyStick_X);  //  X
  y = analogRead(JoyStick_Y);  //  Y

  buttonJoystick = digitalRead(JoyStick_button);
  button1 = digitalRead(main_button1);
  button2 = digitalRead(main_button2);

  x2 = analogRead(JoyStick2_X);  //  X
  y2 = analogRead(JoyStick2_Y);  //  Y

  buttonJoystick2 = digitalRead(JoyStick2_button);
 
  poten = analogRead(poten_Rigth);
 
 Serial.print("x2: " + String(x2));
 Serial.print("\t y2: " + String(y2));
 Serial.print("\t btn2: " + String(buttonJoystick2));
 Serial.print("\t poten: " + String(poten));

   touchEstado =  digitalRead(aux_button1);//touchRead(2);
  touchEstado2 =  digitalRead(aux_button2);// touchRead(15);

  // if (touchEstado < 20) {
  //   touchEstado = 1;
  // } else {
  //   touchEstado = 0;
  // }
Serial.print("\t tb1: " + String(touchEstado));
  Serial.println("\t tb2: " + String(touchEstado2));
  // if (touchEstado2 < 15) {
  //   touchEstado2 = 1;
  // } else {
  //   touchEstado2 = 0;
  // }

  String Data = "";
  if (x < start_down && xp == 0) {

    x = 1;  //map(x, 0, start_down, 1, 4);

    //x = 2;  // 10 - x + 3;
    xp = 2;
    Data += "XB:" + String(x) + "|";

  } else if (x > start_up && xp == 0) {
    x = 1;  //map(x, start_up, 4095, 1, 4);
    xp = -2;
    Data += "XA:" + String(x) + "|";

    //xp != 0 && x > start_down && x < start_up
  } else if (xp != 0 && x > start_down && x < start_up) {

    if (xp < 0) {
      Data += "XA:0|";
    } else {
      Data += "XB:0|";
    }
    xp = 0;
  }

  if (y < start_down && yp == 0) {
    y = 1;  //map(y, 0, start_down, 1, 4);
    //y = 2;  //10 - y + 3;
    yp = 1;
    Data += "YB:" + String(y) + "|";

  } else if (y > start_up && yp == 0) {

    y = 1;  //map(y, start_up, 4095, 1, 4);
    yp = -2;
    Data += "YA:" + String(y) + "|";

  } else if (yp != 0 && y > start_down && y < start_up) {


    if (yp < 0) {
      Data += "YA:0|";
    } else {
      Data += "YB:0|";
    }
    yp = 0;
  }

  if (buttonJoystickP != buttonJoystick) {
    Data += "BJ:" + String(buttonJoystick) + "|";
    buttonJoystickP = buttonJoystick;
  }

  if (button1P != button1) {
    Data += "B1:" + String(button1) + "|";
    button1P = button1;
  }

  if (button2P != button2) {
    Data += "B2:" + String(button2) + "|";
    button2P = button2;
  }

   touchEstado =  digitalRead(aux_button1);//touchRead(2);
    if (touchEstadoP != touchEstado) {
    Data += "TB:" + String(touchEstado) + "|";
    touchEstadoP = touchEstado;
  }

  touchEstado2 =  digitalRead(aux_button2);// touchRead(15);
    if (touchEstadoP2 != touchEstado2) {
    Data += "TB2:" + String(touchEstado2) + "|";
    touchEstadoP2 = touchEstado2;
  }

  // if (touchEstadoP != touchEstado) {
  //   Data += "TB:" + String(touchEstado) + "|";
  //   touchEstadoP = touchEstado;
  // }

  // if (touchEstadoP2 != touchEstado2) {
  //   Data += "TB2:" + String(touchEstado2) + "|";
  //   touchEstadoP2 = touchEstado2;
  // }

  return Data;
}

String readSensores2() {
  x = analogRead(JoyStick_X);  //  X
  y = analogRead(JoyStick_Y);  //  Y

  buttonJoystick = digitalRead(JoyStick_button);
  button1 = digitalRead(main_button1);
  button2 = digitalRead(main_button2);

  updatePantallaLuz01("ejeX", String(x) + "              ");
  updatePantallaLuz01("ejeY", String(y) + "              ");

  String Data = "";
  if (x < start_down && xp == 0) {
    x = 2;  //map(x, 0, start_down, 1, 4);

    xp = 2;
    Data += "X:L" + String(x) + "|";

  } else if (x > start_up && xp == 0) {
    x = 2;  //map(x, start_up, 4095, 1, 4);

    xp = -2;
    Data += "X:R" + String(x) + "|";

  } else if (xp != 0 && x > start_down && x < start_up) {
    if (xp < 0) {
      Data += "X:R0|";
    } else {
      Data += "X:L0|";
    }
    xp = 0;
  }

  if (y < start_down && yp == 0) {
    y = 2;  // map(y, 0, start_down, 1, 4);

    yp = 2;
    Data += "Y:D" + String(y) + "|";

  } else if (y > start_up && yp == 0) {
    y = 2;  //map(y, start_up, 4095, 1, 4);
    yp = -2;
    Data += "Y:U" + String(y) + "|";

  } else if (yp != 0 && y > start_down && y < start_up) {
    if (yp < 0) {
      Data += "Y:U0|";
    } else {
      Data += "Y:D0|";
    }
    yp = 0;
  }

  if (buttonJoystickP != buttonJoystick) {
    Data += "BJ:" + String(buttonJoystick) + "|";
    buttonJoystickP = buttonJoystick;
  }

  if (button1P != button1) {
    Data += "B1:" + String(button1) + "|";
    button1P = button1;
    //goToSleep();
  }

  if (button2P != button2) {
    Data += "B2:" + String(button2) + "|";
    button2P = button2;
  }

  updatePantallaLuz01("sendData", Data);

  return Data;
}