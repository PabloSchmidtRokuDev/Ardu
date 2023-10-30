
int LED_R_Pulse_value = 0;
int LED_B_Pulse_value = 0;
int LED_G_Pulse_value = 0;
int LED_W_Pulse_value = 0;
int LED_Y_Pulse_value = 0;

bool setLedColor() {
  if (brillo_goto != 0){
    tiempo2 = millis();
    if (tiempo2 > (tiempo1 + 100)) {  //Si ha pasado 1 segundo ejecuta el IF
      tiempo1 = millis();
      brillo += brillo_goto;
      if (brillo > 100) brillo = 100;
      if (brillo < 5) brillo = 5;

      LED_R_Brillo_value = LED_R_value * brillo / 100;
      LED_G_Brillo_value = LED_G_value * brillo / 100;
      LED_B_Brillo_value = LED_B_value * brillo / 100;

      Serial.println(" - valor: " + String(brillo) + " - \t LED_R_Brillo_value: " + String(LED_R_Brillo_value)  + "  \t LED_G_Brillo_value: " + String(LED_G_Brillo_value) + "  \t LED_B_Brillo_value" +String(LED_B_Brillo_value) );
    }
  }

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

if (LED_Y_Current_value != LED_Y_Brillo_value) {
    if (LED_Y_Current_value < LED_Y_Brillo_value) LED_Y_Current_value++;
    else LED_Y_Current_value--;
    flag = false;

  }

  if (flag == false) {

    LED_R_Pulse_value = map(LED_R_Current_value, 0, 255, 0, 80);
    LED_B_Pulse_value = map(LED_B_Current_value, 0, 255, 0, 254);
    LED_G_Pulse_value = map(LED_G_Current_value, 0, 255, 0, 254);
    LED_W_Pulse_value = map(LED_W_Current_value, 0, 255, 0, 254);
    LED_Y_Pulse_value = map(LED_Y_Current_value, 0, 255, 0, 254);

    analogWrite(pin_R, LED_R_Pulse_value);
    analogWrite(pin_B, LED_B_Pulse_value);
    analogWrite(pin_G, LED_G_Pulse_value);
    analogWrite(pin_W, LED_W_Pulse_value);
    analogWrite(pin_Y, LED_Y_Pulse_value);

    delayMicroseconds(15000);
  }
 

  return flag;
}