int LED_R_Pulse_value = 0;
int LED_B_Pulse_value = 0;
int LED_G_Pulse_value = 0;
int LED_W_Pulse_value = 0;
int LED_Y_Pulse_value = 0;

int colores[111][3] = {{240, 248, 25},
  {153, 102, 20},
  {250, 235, 21},
  {0, 255, 25},
  {127, 255, 21},
  {240, 255, 25},
  {245, 245, 22},
  {255, 228, 19},
  {0, 0,0 },
  {255, 235, 20},
  {0, 0, 25},
  {138, 43, 22},
  {165, 42, 4},
  {222, 184, 13},
  {95, 158, 16},
  {127, 255, 0},
  {210, 105, 3},
  {255, 127, 8},
  {100, 149, 23},
  {255, 248, 22},
  {220, 20, 6},
  {0, 255, 25},
  {0, 0, 13},
  {0, 139, 13},
  {184, 134, 1},
  {169, 169, 16},
  {169, 169, 16},
  {0, 100,05},
  {189, 183, 10},
  {139, 0, 13},
  {85, 107, 4},
  {255, 140,5 },
  {153, 50, 20},
  {139, 0, 0},
  {233, 150, 12},
  {143, 188, 14},
  {72, 61, 13},
  {47, 79, 7},
  {47, 79, 7},
  {0, 206, 20},
  {148, 0, 21},
  {255, 20, 14},
  {0, 191, 25},
  {105, 105, 10},
  {105, 105, 10},
  {30, 144, 25},
  {178, 34, 3},
  {255, 250, 24},
  {34, 139, 3},
  {255, 0, 25},
  {220, 220, 22},
  {248, 248, 25},
  {255, 215, 0},
  {218, 165, 3},
  {128, 128, 12},
  {128, 128, 12},
  {0, 128, 0},
  {173, 255, 4},
  {240, 255, 24},
  {255, 105, 18},
  {205, 92, 9},
  {75, 0, 13},
  {255, 255, 24},
  {240, 230, 14},
  {230, 230, 25},
  {255, 240, 24},
  {124, 252,5 },
  {255, 250, 20},
  {173, 216, 23},
  {240, 128, 12},
  {224, 255, 25},
  {250, 250, 21},
  {144, 238, 14},
  {211, 211, 21},
  {255, 182, 19},
  {255, 160, 12},
  {32, 178, 17},
  {135, 206, 25},
  {119, 136, 15},
  {119, 136, 15},
  {176, 196, 222},
  {255, 255, 224},
  {0, 255, 0},
  {50, 205, 50},
  {250, 240, 230},
  {255, 0, 255},
  {128, 0, 0},
  {102, 205, 170},
  {0, 0, 205},
  {186, 85, 211},
  {147, 112, 219},
  {60, 179, 113},
  {123, 104, 238},
  {0, 250, 154},
  {72, 209, 204},
  {199, 21, 133},
  {25, 25, 112},
  {245, 255, 250},
  {255, 228, 225},
  {255, 228, 181},
  {255, 222, 173},
  {0, 0, 128},
  {253, 245, 230},
  {128, 128, 0},
  {107, 142, 35},
  {255, 165, 0},
  {255, 69, 0},
  {218, 112, 214},
  {238, 232, 170},
  {152, 251, 152},
  {175, 238, 238}};

bool setLedColor() {
  // if (brillo_goto != 0){
  //   tiempo2 = millis();
  //   if (tiempo2 > (tiempo1 + 100)) {  //Si ha pasado 1 segundo ejecuta el IF
  //     tiempo1 = millis();
  //     brillo += brillo_goto;
  //     if (brillo > 100) brillo = 100;
  //     if (brillo < 5) brillo = 5;

  //     LED_R_Brillo_value = LED_R_value * brillo / 100;
  //     LED_G_Brillo_value = LED_G_value * brillo / 100;
  //     LED_B_Brillo_value = LED_B_value * brillo / 100;

  //     Serial.println(" - valor: " + String(brillo) + " - \t LED_R_Brillo_value: " + String(LED_R_Brillo_value)  + "  \t LED_G_Brillo_value: " + String(LED_G_Brillo_value) + "  \t LED_B_Brillo_value" +String(LED_B_Brillo_value) );
  //   }
  // }

  bool flag = true;

  if (LED_R_Current_value != LED_R_Brillo_value) {
    if (LED_R_Current_value < LED_R_Brillo_value) LED_R_Current_value++;
    else LED_R_Current_value--;
    flag = false;
  }

  if (LED_G_Current_value != LED_G_Brillo_value) {
    if (LED_G_Current_value < LED_G_Brillo_value) LED_G_Current_value++;
    else LED_G_Current_value--;
    flag = false;
  }

  if (LED_B_Current_value != LED_B_Brillo_value) {
    if (LED_B_Current_value < LED_B_Brillo_value) LED_B_Current_value++;
    else LED_B_Current_value--;
    flag = false;
  }




  if (LED_Y_Current_value != LED_Y_Brillo_value) {
    if (LED_Y_Current_value < LED_Y_Brillo_value) LED_Y_Current_value++;
    else LED_Y_Current_value--;
    flag = false;
  }

if (LED_W_Current_value != LED_W_Brillo_value) {
    if (LED_W_Current_value < LED_W_Brillo_value) LED_W_Current_value++;
    else LED_W_Current_value--;
    flag = false;

  }

  if (flag == false) {

    LED_R_Pulse_value = map(LED_R_Current_value, 0, 255, 0, 80);
    LED_B_Pulse_value = map(LED_B_Current_value, 0, 255, 0, 254);
    LED_G_Pulse_value = map(LED_G_Current_value, 0, 255, 0, 254);
    LED_W_Pulse_value = map(LED_W_Current_value, 0, 255, 0, 254);
    //LED_Y_Pulse_value = map(LED_Y_Current_value, 0, 255, 0, 254);

    analogWrite(pin_R, LED_R_Pulse_value);
    analogWrite(pin_B, LED_B_Pulse_value);
    analogWrite(pin_G, LED_G_Pulse_value);
    analogWrite(pin_W, LED_W_Pulse_value);

    analogWrite(pin_R2, LED_R_Pulse_value);
    analogWrite(pin_B2, LED_B_Pulse_value);
    analogWrite(pin_G2, LED_G_Pulse_value);
    analogWrite(pin_W2, LED_W_Pulse_value);
    //analogWrite(pin_Y, LED_Y_Pulse_value);

    delayMicroseconds(15000);
  }
 

  return flag;
}