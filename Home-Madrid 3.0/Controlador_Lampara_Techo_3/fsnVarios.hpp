void setLedColor() {

  int flag = 0;
  for (int i = 0; i < 4; i++) {
    LED_Current_value[i] = LED_RealColor_value[i] * LED_Brillo_value[i] / 100;
    Serial.print(" Brillo " + String(i) + " - Current: " + String(LED_Current_value[i]));
    Serial.print(" \t real: " + String(LED_RealColor_value[i]));
    Serial.println(" \t Brillo: " + String(LED_Brillo_value[i]));

  }

  while (flag == 0) {
    for (int i = 0; i < 4; i++) {

      if (LED_Pulse_value[i] < LED_Current_value[i]) {
        LED_Pulse_value[i]++;
        if (i == 0) analogWrite(LED_Pins[0], map(LED_Pulse_value[0], 0, 255, 0, 20));
        else
          analogWrite(LED_Pins[i], LED_Pulse_value[i]);

        flag = 1;
      }

      if (LED_Pulse_value[i] > LED_Current_value[i]) {
        LED_Pulse_value[i]--;

        if (i == 0) analogWrite(LED_Pins[0], map(LED_Pulse_value[0], 0, 255, 0, 20));
        else
          analogWrite(LED_Pins[i], LED_Pulse_value[i]);

        flag = 1;
      }
    }

    if (flag == 0) flag = 2;
    if (flag == 1) flag = 0;

    delay(10);
  }
}

void ajustarBrillo(int valor) {
  for (int i = 0; i < 4; i++) {
     LED_Brillo_value[i] = valor;
  }
  setLedColor();
}

void ColorFijoClaro() {
  LED_RealColor_value[0] = 0;
  LED_RealColor_value[1] = 0;
  LED_RealColor_value[2] = 0;
  LED_RealColor_value[3] = 255;
  ajustarBrillo(100);
}

void ColorFijo() {
  LED_RealColor_value[0] = random(0, 255);
  LED_RealColor_value[1] = random(0, 255);
  LED_RealColor_value[2] = random(0, 255);
  LED_RealColor_value[3] = random(10, 50);
  setLedColor();
}

void ApagarLed() {
  Serial.println("cargando leds");
  // colorX ccl = colorXList[random(0, 52)];
  LED_RealColor_value[0] = 0;
  LED_RealColor_value[1] = 0;
  LED_RealColor_value[2] = 0;
  LED_RealColor_value[3] = 0;
  setLedColor();
}



// int setLedColor() {
//   int flag = 0;
//   // tiempoLed2 = millis();
//   // if (tiempoLed2 >= tiempoLed1 + 2) {

//     for (int i = 0; i < 4; i++) {

//       if (LED_Pulse_value[i] < LED_Current_value[i]) {
//         LED_Pulse_value[i]++;

//         flag = 1;
//         break;
//       }

//       if (LED_Pulse_value[i] > LED_Current_value[i]) {
//         LED_Pulse_value[i]--;

//         flag = 1;
//          break;
//       }

//     }

//       if (flag == 1) {
//         analogWrite(ledPin1, map(LED_Pulse_value[0], 0, 250, 0, 20));
//         analogWrite(ledPin2, LED_Pulse_value[1]);
//         analogWrite(ledPin3, LED_Pulse_value[2]);
//         analogWrite(ledPin4, LED_Pulse_value[3]);
//       }

//     if (flag == 0) {

//       flag = 2;
//     }
//   //   tiempoLed1 = millis();
//   // }

//   return flag;
// }

// int pulseWidth(int angle) {
//   int pulse_wide, analog_value;
//   pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
//   analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
//   return analog_value;
// }

// float maxCos = 6.3;
// void EfectoOnda() {

//   float realCos;
//   float realSpos;
//   for (int i = 0; i < 6; i++) {
//     estado_servo_cos[i]++;
//     realCos = (cos(estado_servo_cos[i]) + 1) / 2;
//     realSpos = 1 * (100 - 10) * realCos + 10;

//     goto_servo[i] = realSpos;

//     Serial.print("  goto_servo: " + String(realSpos));
//     Serial.print("  s: " + String(i));
//     if (estado_servo_cos[i] == maxCos) estado_servo_cos[i] = 0;
//   }
//   Serial.println("   ");
// }



// void ServosPos(int posX) {
//   if (posX == 0) {
//     for (int i = 0; i < 6; i++) {
//       goto_servo[i] = 0;
//     }
//   } else if (posX == 1) {
//     for (int i = 0; i < 6; i++) {
//       goto_servo[i] = 90;
//     }
//   } else if (posX == 2) {
//     for (int i = 0; i < 6; i++) {
//       if (i % 2 == 0)
//         goto_servo[i] = 80;
//       else
//         goto_servo[i] = 10;
//     }
//   } else if (posX == 3) {
//     for (int i = 0; i < 6; i++) {
//       if (i % 3 == 0)
//         goto_servo[i] = 50;
//       else
//         goto_servo[i] = 10;
//     }
//   } else if (posX == 4) {
//     for (int i = 0; i < 6; i++) {
//       if (i % 2 == 0)
//         goto_servo[i] = 10;
//       else
//         goto_servo[i] = 60;
//     }
//   }

//   tiempoMotor1 = millis();
// }

// int posServos = 0;

// void setServosPos() {
//   ServosPos(random(0, 4));
// }

// bool attached = false;

// int mover_servos() {
//   int flag = 0;
//   tiempoMotor2 = millis();
//   if (tiempoMotor2 >= tiempoMotor1 + 100) {


//     for (int i = 0; i <= 5; i++) {
//       if (estado_servo[i] < goto_servo[i]) {
//         estado_servo[i]++;
//         flag = 1;
//         //break;
//       }

//       if (estado_servo[i] > goto_servo[i]) {
//         estado_servo[i]--;
//         flag = 1;
//         //break;
//       }
//     }

//     if (flag == 1) {
//       if (attached == false) {
//         // myservo1.attach(servoPin1);
//         // myservo2.attach(servoPin2);
//         // myservo3.attach(servoPin3);
//         // myservo4.attach(servoPin4);
//         // myservo5.attach(servoPin5);
//         // myservo6.attach(servoPin6);
//         attached = true;
//         // delay(100);
//       }
//       myservo1.write(estado_servo[0]);
//       myservo2.write(estado_servo[1]);
//       myservo3.write(estado_servo[2]);
//       myservo4.write(estado_servo[3]);
//       myservo5.write(estado_servo[4]);
//       // myservo6.write(estado_servo[5]);
//     }


//     if (flag == 0) {

//       flag = 2;
//       // delay(1000);
//       // myservo1.detach();
//       // myservo2.detach();
//       // myservo3.detach();
//       // myservo4.detach();
//       // myservo5.detach();
//       // myservo6.detach();
//       attached = false;
//     }
//     tiempoMotor1 = millis();
//   }

//   return flag;
// }
