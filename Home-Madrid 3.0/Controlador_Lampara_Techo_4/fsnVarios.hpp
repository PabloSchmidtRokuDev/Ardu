void setLedColor() {

  int flag = 0;

  while (flag == 0) {
    for (int i = 0; i < 4; i++) {

      if (LED_Current_value[i] < LED_Brillo_value[i]) {
        LED_Current_value[i]++;
        if (i == 0) analogWrite(LED_Pins[i], map(LED_Current_value[0], 0, 255, 0, 20));
        else
          analogWrite(LED_Pins[i], LED_Current_value[i]);

        flag = 1;
      }

      if (LED_Current_value[i] > LED_Brillo_value[i]) {
        LED_Current_value[i]--;

        if (i == 0) analogWrite(LED_Pins[i], map(LED_Current_value[0], 0, 255, 0, 20));
        else
          analogWrite(LED_Pins[i], LED_Current_value[i]);

        flag = 1;
      }
    }

    if (flag == 0) flag = 2;
    if (flag == 1) flag = 0;

    delay(10);
  }
}

void ColorFijo() {
  LED_Brillo_value[0] = random(0, 255);
  LED_Brillo_value[1] = random(0, 255);
  LED_Brillo_value[2] = random(0, 255);
  LED_Brillo_value[3] = random(10, 50);
  setLedColor();
}

void ApagarLed() {
  Serial.println("cargando leds");
  // colorX ccl = colorXList[random(0, 52)];
  LED_Brillo_value[0] = 0;
  LED_Brillo_value[1] = 0;
  LED_Brillo_value[2] = 0;
  LED_Brillo_value[3] = 0;
  setLedColor();
}


