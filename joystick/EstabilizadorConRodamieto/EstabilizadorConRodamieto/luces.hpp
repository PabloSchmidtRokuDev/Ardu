
void setLedColor() {
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

  while (LED_R_Current_value != LED_R_Brillo_value || LED_G_Current_value != LED_G_Brillo_value || LED_B_Current_value != LED_B_Brillo_value) {
    //cTiempo1 = millis();

    if (LED_R_Current_value != LED_R_Brillo_value) {
      if (LED_R_Current_value < LED_R_Brillo_value) LED_R_Current_value++;
      else LED_R_Current_value--;
    }

    if (LED_G_Current_value != LED_G_Brillo_value) {
      if (LED_G_Current_value < LED_G_Brillo_value) LED_G_Current_value++;
      else LED_G_Current_value--;
    }

    if (LED_B_Current_value != LED_B_Brillo_value) {
      if (LED_B_Current_value < LED_B_Brillo_value) LED_B_Current_value++;
      else LED_B_Current_value--;
    }
/*    analogWrite(pin_R, LED_R_Current_value);
    analogWrite(pin_B, LED_B_Current_value);
    analogWrite(pin_G, LED_G_Current_value);
    */
    delayMicroseconds(1000);
  }
}