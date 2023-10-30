int LED_R_o_Pulse_value = 0;
int LED_B_o_Pulse_value = 0;
int LED_G_o_Pulse_value = 0;

int LED_R_Pulse_value = 0;
int LED_B_Pulse_value = 0;
int LED_G_Pulse_value = 0;
int LED_W_Pulse_value = 0;
int LED_Y_Pulse_value = 0;

int setLedColor() {
  int flag = 0;

 //tiempoLed2 = millis();

  if (LED_R_o_Current_value != LED_R_o_Brillo_value) {
    if (LED_R_o_Current_value < LED_R_o_Brillo_value) LED_R_o_Current_value++;
    else LED_R_o_Current_value--;
    flag = 1;
  }

  if (LED_G_o_Current_value != LED_G_o_Brillo_value) {
    if (LED_G_o_Current_value < LED_G_o_Brillo_value) LED_G_o_Current_value++;
    else LED_G_o_Current_value--;
    flag = 1;
  }

  if (LED_B_o_Current_value != LED_B_o_Brillo_value) {
    if (LED_B_o_Current_value < LED_B_o_Brillo_value) LED_B_o_Current_value++;
    else LED_B_o_Current_value--;
    flag = 1;
  }


  if (LED_R_Current_value != LED_R_Brillo_value) {
    if (LED_R_Current_value < LED_R_Brillo_value) LED_R_Current_value++;
    else LED_R_Current_value--;
    flag = 1;
  }

  if (LED_G_Current_value != LED_G_Brillo_value) {
    if (LED_G_Current_value < LED_G_Brillo_value) LED_G_Current_value++;
    else LED_G_Current_value--;
    flag = 1;
  }

  if (LED_B_Current_value != LED_B_Brillo_value) {
    if (LED_B_Current_value < LED_B_Brillo_value) LED_B_Current_value++;
    else LED_B_Current_value--;
    flag = 1;
  }


  if (LED_Y_Current_value != LED_Y_Brillo_value) {
    if (LED_Y_Current_value < LED_Y_Brillo_value) LED_Y_Current_value++;
    else LED_Y_Current_value--;
    flag = 1;
  }

  if (LED_W_Current_value != LED_W_Brillo_value) {
    if (LED_W_Current_value < LED_W_Brillo_value) LED_W_Current_value++;
    else LED_W_Current_value--;
    flag = 1;
  }

  if (LED_Y_Current_value != LED_Y_Brillo_value) {
    if (LED_Y_Current_value < LED_Y_Brillo_value) LED_Y_Current_value++;
    else LED_Y_Current_value--;
    flag = 1;
  }

  if (flag == 1) {
    LED_R_o_Pulse_value = map(LED_R_o_Current_value, 0, 255, 0, 10);
    LED_B_o_Pulse_value = map(LED_B_o_Current_value, 0, 255, 0, 10);
    LED_G_o_Pulse_value = map(LED_G_o_Current_value, 0, 255, 0, 10);

    LED_R_Pulse_value = map(LED_R_Current_value, 0, 255, 0, 10);
    LED_B_Pulse_value = map(LED_B_Current_value, 0, 255, 0, 10);
    LED_G_Pulse_value = map(LED_G_Current_value, 0, 255, 0, 10);
    LED_W_Pulse_value = map(LED_W_Current_value, 0, 255, 0, 10);
    LED_Y_Pulse_value = map(LED_Y_Current_value, 0, 255, 0, 10);


    analogWrite(pin_R_o, LED_R_o_Pulse_value);
    analogWrite(pin_B_o, LED_B_o_Pulse_value);
    analogWrite(pin_G_o, LED_G_o_Pulse_value);

    analogWrite(pin_R, LED_R_Pulse_value);
    analogWrite(pin_B, LED_B_Pulse_value);
    analogWrite(pin_G, LED_G_Pulse_value);
    analogWrite(pin_W, LED_W_Pulse_value);
    analogWrite(pin_Y, LED_Y_Pulse_value);

    delayMicroseconds(5000);
  }
  else{
    flag = 2;
  }


  return flag;
}