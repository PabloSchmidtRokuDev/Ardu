// A fun MATRIX-like screen demo of scrolling
// Screen will flicker initially until fully drawn
// then scroll smoothly

// Needs GLCD font

/*
 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

#include <Arduino.h>

#define DEBUG_SERIAL Serial

#include "componentes.hpp"
#include "pantalla.hpp"
#include "conexiones.hpp"


String pantallaS = "";

int JoyStick_X = 32;       // Analog Pin  X
int JoyStick_Y = 35;       // // Analog Pin  Y
int JoyStick_button = 25;  // IO Pin

int main_button1 = 33;  // IO Pin
int main_button2 = 33;  // IO Pin

int start_down = 1200;
int start_up = 2100;

#include "colores.hpp"
#include "fsnPantalla.hpp"
#include "city.h"
void setup() {
  Serial.begin(115200);

  pinMode(JoyStick_X, INPUT);
  pinMode(JoyStick_Y, INPUT);

  pinMode(JoyStick_button, INPUT_PULLUP);

  pinMode(main_button1, INPUT_PULLUP);
  //pinMode(main_button2, INPUT_PULLUP);

  randomSeed(analogRead(A0));
  tft.init();
  tft.setRotation(2);
  tft.setSwapBytes(true);
  //tft.fillScreen(TFT_BLACK);


   sprMatrix.createSprite(320, 170);
   sprMatrix.setSwapBytes(true);

 sprMenuOptions.createSprite(80,80);
 sprMenuOptions.setTextColor(TFT_WHITE,TFT_BLACK);

  // sprMenuOptions.createSprite(240, 320);
  //sprMenuOptions.fillScreen(TFT_TRANSPARENT);
  //sprMenuOptions.fillTriangle(25, 0, 0, 49, 49, 49, TFT_RED);
  //sprMenuOptions.pushSprite(20, 20, TFT_TRANSPARENT);

  setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);

  xpos = tft.width() / 2 - 60;  // Half the screen width
  ypos = 10;


  sprMenuOptions.loadFont(AA_FONT_SMALL);              // Must load the font first into the sprite class
  //sprMenuOptions.setTextColor(TFT_YELLOW, TFT_BLACK);  // Set the sprite font colour and the background colour

  //iniciarWifiServer();

}

String DataRepetir = "";

void loop(void) {

   sprMatrix.pushImage(0,0,320,170,city);
   crearMenu();

//arrowSprite.pushImage(0,0,96,96,arrow);
// arrowSprite.pushToSprite(&background,x,40,TFT_BLACK); 
  
 
 sprMatrix.pushSprite(0,0);

  // sprMatrix.pushToSprite(&sprMenuOptions, 0, 0);
  // //First fill the screen with random streaks of characters
  // for (int j = 0; j < 600; j += TEXT_HEIGHT) {
  //   for (int i = 0; i < 40; i++) {
  //     if (pos[i] > 20) pos[i] -= 3;                                      // Rapid fade initially brightness values
  //     if (pos[i] > 0) pos[i] -= 1;                                       // Slow fade later
  //     if ((random(20) == 1) && (j < 400)) pos[i] = 63;                   // ~1 in 20 probability of a new character
  //     sprMatrix.setTextColor(pos[i] << 5, ILI9341_BLACK);                      // Set the green character brightness
  //     if (pos[i] == 63) sprMatrix.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  // Draw white character
  //     xPos += sprMatrix.drawChar(random(32, 128), xPos, yDraw, 1);             // Draw the character
      
  //   }
  //   yDraw = scroll_slow(TEXT_HEIGHT, 2);  // Scroll, 14ms per pixel line
  //   xPos = 0;
  // }
  
  // sprMenuOptions.pushSprite(0, 0);//, TFT_TRANSPARENT);

  // while (1) {
  //  yield();
  //  yDraw = scroll_slow(320, 5);
  // }  // Scroll 320 lines, 5ms per line

/*
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

    if (Data == "" && DataRepetir != "" && subOpcionSelect == 10) {
      Data = DataRepetir;
    }

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
  */
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

    //dataAvailable = dataAvailable.substring(dataAvailable.indexOf("|") + 1);
    //Serial.print(" \t 2- new  dataAvailable: " + dataAvailable);

    String obj = cm.substring(0, cm.indexOf(":"));
    Serial.print("\t obj: " + obj);
    cm = cm.substring(cm.indexOf(":") + 1);
    Serial.println(" \t 3- cm final: " + cm);

    if (obj == "TB" && cm == "0") {
      if (subOpcionSelect == 1) {
        updatePantallaBrazo01("Ejes3y4", "");
      } else if (subOpcionSelect == 2) {

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

    } else if (obj == "BJ" && cm == "0") {
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

        if (obj == "YA" && cm == "1") colorXsel = getColores(dir_rigth);
        if (obj == "YB" && cm == "1") colorXsel = getColores(dir_left);
        if (obj == "XB" && cm == "1") colorXsel = getColores(dir_down);
        if (obj == "XA" && cm == "1") colorXsel = getColores(dir_up);

        if (cm == "1") {
          DataRepetir = dataAvailable;
          delay(50);
          if (colorXsel.brillo != -1) {
            return "BRILLO:" + String(colorXsel.brillo) + "|";;
          } else {

            return "COLOR:" + String(colorXsel.r) + ";" + String(colorXsel.g) + ";" + String(colorXsel.b) + ";" + String(colorXsel.nombre) + "|";
          }

        } else {
          DataRepetir = "";
        }
        return "";
      }
    }
    if (obj == "TB2") {
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

  if (buttonJoystick == 1) buttonJoystick = 0;
  else buttonJoystick = 1;

  String Data = "";
  if (x < start_down && xp == 0) {

    x = 1;  //map(x, 0, start_down, 1, 4);

    //x = 2;  // 10 - x + 3;
    xp = 2;
    Data += "XA:" + String(x) + "|";

  } else if (x > start_up && xp == 0) {
    x = 1;  //map(x, start_up, 4095, 1, 4);
    xp = -2;
    Data += "XB:" + String(x) + "|";

    //xp != 0 && x > start_down && x < start_up
  } else if (xp != 0 && x > start_down && x < start_up) {

    if (xp < 0) {
      Data += "XB:0|";
    } else {
      Data += "XA:0|";
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

  return Data;
}

String readSensores2() {
  x = analogRead(JoyStick_X);  //  X
  y = analogRead(JoyStick_Y);  //  Y

  buttonJoystick = digitalRead(JoyStick_button);
  button1 = digitalRead(main_button1);
  // button2 = digitalRead(main_button2);
  if (buttonJoystick == 1) buttonJoystick = 0;
  else buttonJoystick = 1;
  if (button1 == 1) button1 = 0;
  else button1 = 1;

  updatePantallaLuz01("ejeX", String(x) + "              ");
  updatePantallaLuz01("ejeY", String(y) + "              ");

  String Data = "";
  if (x < start_down && xp == 0) {
    x = 2;  //map(x, 0, start_down, 1, 4);

    xp = 2;
    Data += "X:R" + String(x) + "|";

  } else if (x > start_up && xp == 0) {
    x = 2;  //map(x, start_up, 4095, 1, 4);

    xp = -2;
    Data += "X:L" + String(x) + "|";

  } else if (xp != 0 && x > start_down && x < start_up) {
    if (xp < 0) {
      Data += "X:L0|";
    } else {
      Data += "X:R0|";
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

  // if (button2P != button2) {
  //   Data += "B2:" + String(button2) + "|";
  //   button2P = button2;
  // }

  updatePantallaLuz01("sendData", Data);

  return Data;
}

void setupScrollArea(uint16_t TFA, uint16_t BFA) {
  sprMatrix.writecommand(ILI9341_VSCRDEF);  // Vertical scroll definition
  sprMatrix.writedata(TFA >> 8);
  sprMatrix.writedata(TFA);
  sprMatrix.writedata((320 - TFA - BFA) >> 8);
  sprMatrix.writedata(320 - TFA - BFA);
  sprMatrix.writedata(BFA >> 8);
  sprMatrix.writedata(BFA);
}

int scroll_slow(int lines, int wait) {
  int yTemp = yStart;
  for (int i = 0; i < lines; i++) {
    yStart++;
    if (yStart == 320 - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA;
    scrollAddress(yStart);
    delay(wait);
  }
  return yTemp;
}

void scrollAddress(uint16_t VSP) {
  // sprMatrix.writecommand(ILI9341_VSCRSADD);  // Vertical scrolling start address
  // sprMatrix.writedata(VSP >> 8);
  // sprMatrix.writedata(VSP);

  sprMatrix.pushSprite(0, 320- VSP);//, TFT_TRANSPARENT);

}
