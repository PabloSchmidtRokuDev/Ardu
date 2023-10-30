
int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);

  return analog_value;
}

int estado_servo[6] = { 180, 180, 180, 180, 180, 180 };
float estado_servo_cos[6] = { 0, 1, 2, 3, 4, 5 };
int goto_servo[6] = { 180, 180, 180, 180, 180, 180 };

float maxCos = 6.3;
void Clasico() {

  float realCos;
  float realSpos;
  for (int i = 0; i < 6; i++) {
    estado_servo_cos[i]++;
    realCos = (cos(estado_servo_cos[i]) + 1) / 2;
    realSpos = 1 * (180 - 120) * realCos + 120;

    goto_servo[i] = realSpos;

    // Serial.print("  goto_servo: "+ String(realSpos));
    // Serial.print("  s: "+ String(i));
    if (estado_servo_cos[i] == maxCos) estado_servo_cos[i] = 0;
  }
  //Serial.println("   ");
}

void ServosPos(int posX) {
  if (posX == 0) {
    for (int i = 0; i < 6; i++) {
      goto_servo[i] = 180;
    }
  } else if (posX == 1) {
    for (int i = 0; i < 6; i++) {
      goto_servo[i] = 90;
    }
  } else if (posX == 2) {
    for (int i = 0; i < 6; i++) {
      if (i % 2 == 0)
        goto_servo[i] = 180;
      else
        goto_servo[i] = 70;
    }
  } else if (posX == 3) {
    for (int i = 0; i < 6; i++) {
      if (i % 3 == 0)
        goto_servo[i] = 160;
      else
        goto_servo[i] = 140;
    }
  } else if (posX == 4) {
    for (int i = 0; i < 6; i++) {
      if (i % 2 == 0)
        goto_servo[i] = 160;
      else
        goto_servo[i] = 140;
    }
  }
}

int mover_servos() {
  int flag = 0;
  tiempoMotor2 = millis();
  if (tiempoMotor2 >= tiempoMotor1 + 10) {


    for (int i = 0; i <= 5; i++) {

      if (estado_servo[i] < goto_servo[i]) {
        estado_servo[i]++;
        flag = 1;
      }

      if (estado_servo[i] > goto_servo[i]) {
        estado_servo[i]--;
        flag = 1;
      }

      pwm.setPWM(servoPin[i], 0, pulseWidth(estado_servo[i]));
    }

    if (flag == 0) {

      flag = 2;
    }
    tiempoMotor1 = millis();
  }

  return flag;
}

int setLedColor() {
  int flag = 0;
  tiempoLed2 = millis();
  if (tiempoLed2 >= tiempoLed1 + 2) {

    for (int i = 0; i < 4; i++) {

      if (LED_Current_value[i] < LED_Brillo_value[i]) {
        LED_Current_value[i]++;
        flag = 1;
      }

      if (LED_Current_value[i] > LED_Brillo_value[i]) {
        LED_Current_value[i]--;
        flag = 1;
      }

      if (i == 0) {
        int mapVal = map(LED_Current_value[0], 0, 250, 0, 20);
        Serial.print("  c: "+ String(mapVal));
        Serial.println("  p: "+ String(pulseWidth(mapVal)));

      } else {
        pwm.setPWM(ledPin[i], 500, pulseWidth(LED_Current_value[i]));
      }
    }

    if (flag == 0) {

      flag = 2;
    }
    tiempoLed1 = millis();
  }

  return flag;
}
