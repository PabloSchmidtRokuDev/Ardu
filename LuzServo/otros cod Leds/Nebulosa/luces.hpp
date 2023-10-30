
int LED_R_Pulse_value = 0;
int LED_B_Pulse_value = 0;
int LED_G_Pulse_value = 0;
int LED_W_Pulse_value = 0;
int LED_Y_Pulse_value = 0;

bool setLedColor() {

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


    LED_R_Pulse_value = map(LED_R_Current_value, 0, 255, 0, 180);
    LED_B_Pulse_value = LED_B_Current_value;//map(LED_B_Current_value, 0, 255, 0, 255);
    LED_G_Pulse_value = LED_G_Current_value;//map(LED_G_Current_value, 0, 255, 0, 255);
    LED_W_Pulse_value = LED_W_Current_value;//map(LED_W_Current_value, 0, 255, 0, 255);
    LED_Y_Pulse_value = LED_Y_Current_value;//map(LED_Y_Current_value, 0, 255, 0, 255);


 
    analogWrite(pin_R, LED_R_Pulse_value);
    analogWrite(pin_B, LED_B_Pulse_value);
    analogWrite(pin_G, LED_G_Pulse_value);
    analogWrite(pin_W, LED_W_Pulse_value);
    analogWrite(pin_Y, LED_Y_Pulse_value);

    delayMicroseconds(10000);
  }
 

  return flag;
}