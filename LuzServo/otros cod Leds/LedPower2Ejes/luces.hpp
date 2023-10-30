

int LED_R_Pulse_value = 0;
int LED_B_Pulse_value = 0;
int LED_G_Pulse_value = 0;
int LED_W_Pulse_value = 0;

int setLedColor() {
  int flag = 0;
  tiempoLed2 = millis();
  //if (tiempoLed2 >= tiempoLed1 + 100) {

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

  if (LED_W_Current_value != LED_W_Brillo_value) {
    if (LED_W_Current_value < LED_W_Brillo_value) LED_W_Current_value++;
    else LED_W_Current_value--;
    flag = 1;
  }

  if (flag == 1) {

    LED_R_Pulse_value = map(LED_R_Current_value, 0, 255, 0, 100);
    LED_B_Pulse_value = map(LED_B_Current_value, 0, 255, 0, 200);
    LED_G_Pulse_value = map(LED_G_Current_value, 0, 255, 0, 200);
    LED_W_Pulse_value = map(LED_W_Current_value, 0, 255, 0, 100);

    analogWrite(pin_R, LED_R_Pulse_value);
    analogWrite(pin_B, LED_B_Pulse_value);
    analogWrite(pin_G, LED_G_Pulse_value);
    analogWrite(pin_W, LED_W_Pulse_value);

    // Serial.print("LED_R_Pulse_value.." + String(LED_R_Pulse_value));
    // Serial.print("\t LED_G_Pulse_value.." + String(LED_G_Pulse_value));
    // Serial.print("\t LED_B_Pulse_value.." + String(LED_B_Pulse_value));
    // Serial.println("\t LED_W_Pulse_value.." + String(LED_W_Pulse_value));
    delay(10);
    return 1;
  } else {
    // Serial.println("sin cambio..");
    return 2;
  }


  tiempoLed1 = millis();
  //  }
  return 0;
}