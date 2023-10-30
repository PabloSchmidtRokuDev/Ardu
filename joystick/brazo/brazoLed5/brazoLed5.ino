#include <Servo.h>

Servo servo_base;
Servo servo_flexionn1;
Servo servo_flexionn2;
Servo servo_Lampara;
/*
const int pin_servo_base = 4;
const int pin_servo_flexionn1 = 5;
const int pin_servo_flexionn2 = 6;
const int pin_servo_Lampara = 7;

*/
  const int pin_servo_base = 2;
  const int pin_servo_flexionn1 = 0;
  const int pin_servo_flexionn2 = 4;
  const int pin_servo_Lampara = 5;


const int pin_Led_R = 15;
const int pin_Led_G = 13;
const int pin_Led_B = 12;
const int pin_Led_WC = 14;
const int pin_Led_WW = 16;

int estado_servo_base = 90;
int estado_servo_flexionn1 = 90;
int estado_servo_flexionn2 = 90;
int estado_servo_Lampara = 90;

int goto_servo_base = 90;
int goto_servo_flexionn1 = 90;
int goto_servo_flexionn2 = 90;
int goto_servo_Lampara = 90;

int velocidadServo = 1;

void setup() {
  // put your setup code here, to run once:

  servo_base.attach(pin_servo_base);
  servo_flexionn1.attach(pin_servo_flexionn1);
  servo_flexionn2.attach(pin_servo_flexionn2);
  servo_Lampara.attach(pin_servo_Lampara);

  pinMode(pin_Led_R, OUTPUT);
  pinMode(pin_Led_G, OUTPUT);
  pinMode(pin_Led_B, OUTPUT);
  pinMode(pin_Led_WC, OUTPUT);
  pinMode(pin_Led_WW, OUTPUT);

  servo_base.write(90);
  servo_flexionn1.write(90);
  servo_flexionn2.write(90);
  servo_Lampara.write(90);

  Serial.begin(115200);
  Serial.println("start...");
}


void loop() {

  digitalWrite(pin_Led_R, 255);
  digitalWrite(pin_Led_G, 0);
  digitalWrite(pin_Led_B, 0);
  digitalWrite(pin_Led_WC, 0);
  digitalWrite(pin_Led_WW, 0);

  mover_servo_base(180);
  mover_servo_flexionn1(180);
  mover_servo_flexionn2(180);
  mover_servo_Lampara(180);
  //mover_servos();
  delay(1000);

  digitalWrite(pin_Led_R, 0);
  digitalWrite(pin_Led_G, 255);
  digitalWrite(pin_Led_B, 0);
  digitalWrite(pin_Led_WC, 0);
  digitalWrite(pin_Led_WW, 0);

  mover_servo_base(0);
  mover_servo_flexionn1(0);
  mover_servo_flexionn2(0);
  mover_servo_Lampara(0);
  //mover_servos();
  delay(1000);


  digitalWrite(pin_Led_R, 0);
  digitalWrite(pin_Led_G, 0);
  digitalWrite(pin_Led_B, 255);
  digitalWrite(pin_Led_WC, 0);
  digitalWrite(pin_Led_WW, 0);

  mover_servo_base(90);
  mover_servo_flexionn1(90);
  mover_servo_flexionn2(90);
  mover_servo_Lampara(90);
  //mover_servos();
  delay(1000);

  digitalWrite(pin_Led_R, 0);
  digitalWrite(pin_Led_G, 0);
  digitalWrite(pin_Led_B, 0);
  digitalWrite(pin_Led_WC, 255);
  digitalWrite(pin_Led_WW, 0);

  mover_servo_base(90);
  mover_servo_flexionn1(130);
  mover_servo_flexionn2(180);
  mover_servo_Lampara(150);
  //mover_servos();
  delay(1000);

  digitalWrite(pin_Led_R, 0);
  digitalWrite(pin_Led_G, 0);
  digitalWrite(pin_Led_B, 0);
  digitalWrite(pin_Led_WC, 0);
  digitalWrite(pin_Led_WW, 255);

  mover_servo_base(60);
  mover_servo_flexionn1(130);
  mover_servo_flexionn2(10);
  mover_servo_Lampara(180);
  //mover_servos();
  delay(1000);
  /*
    digitalWrite(pin_Led_R, 0);
    digitalWrite(pin_Led_G, 170);
    digitalWrite(pin_Led_B, 0);
    digitalWrite(pin_Led_WC, 250);
    digitalWrite(pin_Led_WW, 0);

    mover_servo_base(90);
    mover_servo_flexionn1(90);
    mover_servo_flexionn2(90);
    mover_servo_Lampara(90);
    mover_servos();
    delay(1000);
  */
}

void mover_servo_flexionn1(int pos) {
  goto_servo_flexionn1 = pos;

  
  int mover_flexionn1 = estado_servo_flexionn1 - goto_servo_flexionn1;
  mover_flexionn1 = mover_flexionn1 * -1;

  bool servosEnPosicion = estado_servo_flexionn1 == goto_servo_flexionn1;
  
  while (servosEnPosicion == false)
  {
    if (estado_servo_flexionn1 != goto_servo_flexionn1) {
      if (estado_servo_flexionn1 < goto_servo_flexionn1) {
        estado_servo_flexionn1++;
      } else {
        estado_servo_flexionn1--;
      }
      servo_flexionn1.write(estado_servo_flexionn1);
    }


    delay(velocidadServo);

    servosEnPosicion = estado_servo_flexionn1 == goto_servo_flexionn1;

  }
}

void mover_servo_flexionn2(int pos) {
  goto_servo_flexionn2 = pos;


  int mover_flexionn2 = estado_servo_flexionn2 - goto_servo_flexionn2;
  mover_flexionn2 = mover_flexionn2 * -1;


  bool servosEnPosicion = estado_servo_flexionn2 == goto_servo_flexionn2;
  
  while (servosEnPosicion == false)
  {

    if (estado_servo_flexionn2 != goto_servo_flexionn2) {
      if (estado_servo_flexionn2 < goto_servo_flexionn2) {
        estado_servo_flexionn2++;
      } else {
        estado_servo_flexionn2--;
      }
      servo_flexionn2.write(estado_servo_flexionn2);
    }


    delay(velocidadServo);

    servosEnPosicion = estado_servo_flexionn2 == goto_servo_flexionn2;
  }
}

void mover_servo_Lampara(int pos) {
  goto_servo_Lampara = pos;


  int mover_Lampara = estado_servo_Lampara - goto_servo_Lampara;
  mover_Lampara = mover_Lampara * -1;

  bool servosEnPosicion = estado_servo_Lampara == goto_servo_Lampara;

  while (servosEnPosicion == false)
  {

    if (estado_servo_Lampara != goto_servo_Lampara) {
      if (estado_servo_Lampara < goto_servo_Lampara) {
        estado_servo_Lampara++;
      } else {
        estado_servo_Lampara--;
      }
      servo_Lampara.write(estado_servo_Lampara);
    }

    delay(velocidadServo);

    servosEnPosicion = estado_servo_Lampara == goto_servo_Lampara;

  }
}

void mover_servo_base(int pos) {
  goto_servo_base = pos;

    int mover_base = estado_servo_base - goto_servo_base;
  mover_base = mover_base * -1;

  bool servosEnPosicion = estado_servo_base == goto_servo_base;

   while (servosEnPosicion == false)
  {

    if (estado_servo_base != goto_servo_base) {
      if (estado_servo_base < goto_servo_base) {
        estado_servo_base++;
      } else {
        estado_servo_base--;
      }
      servo_base.write(estado_servo_base);
    }


    delay(velocidadServo);

    servosEnPosicion = estado_servo_base == goto_servo_base;

  }
  
}


/*
void mover_servos() {

  int mover_base = estado_servo_base - goto_servo_base;
  mover_base = mover_base * -1;

  int mover_flexionn1 = estado_servo_flexionn1 - goto_servo_flexionn1;
  mover_flexionn1 = mover_flexionn1 * -1;

  int mover_flexionn2 = estado_servo_flexionn2 - goto_servo_flexionn2;
  mover_flexionn2 = mover_flexionn2 * -1;

  int mover_Lampara = estado_servo_Lampara - goto_servo_Lampara;
  mover_Lampara = mover_Lampara * -1;

  bool servosEnPosicion = estado_servo_base == goto_servo_base;
  servosEnPosicion = servosEnPosicion && estado_servo_flexionn1 == goto_servo_flexionn1;
  servosEnPosicion = servosEnPosicion && estado_servo_flexionn2 == goto_servo_flexionn2;
  servosEnPosicion = servosEnPosicion && estado_servo_Lampara == goto_servo_Lampara;

  int vuelas = 0;

  while (servosEnPosicion == false)
  {

    if (estado_servo_base != goto_servo_base) {
      if (estado_servo_base < goto_servo_base) {
        estado_servo_base++;
      } else {
        estado_servo_base--;
      }
      servo_base.write(estado_servo_base);
    }

    if (estado_servo_flexionn1 != goto_servo_flexionn1) {
      if (estado_servo_flexionn1 < goto_servo_flexionn1) {
        estado_servo_flexionn1++;
      } else {
        estado_servo_flexionn1--;
      }
      servo_flexionn1.write(estado_servo_flexionn1);
    }

    if (estado_servo_flexionn2 != goto_servo_flexionn2) {
      if (estado_servo_flexionn2 < goto_servo_flexionn2) {
        estado_servo_flexionn2++;
      } else {
        estado_servo_flexionn2--;
      }
      servo_flexionn2.write(estado_servo_flexionn2);
    }

    if (estado_servo_Lampara != goto_servo_Lampara) {
      if (estado_servo_Lampara < goto_servo_Lampara) {
        estado_servo_Lampara++;
      } else {
        estado_servo_Lampara--;
      }
      servo_Lampara.write(estado_servo_Lampara);
    }

    delay(velocidadServo);

    servosEnPosicion = estado_servo_base == goto_servo_base;
    servosEnPosicion = servosEnPosicion && estado_servo_flexionn1 == goto_servo_flexionn1;
    servosEnPosicion = servosEnPosicion && estado_servo_flexionn2 == goto_servo_flexionn2;
    servosEnPosicion = servosEnPosicion && estado_servo_Lampara == goto_servo_Lampara;

    vuelas++;
  }

  Serial.print("vuelas: ");
  Serial.print(vuelas);

  Serial.print(" - estado_servo_flexionn1: ");
  Serial.print(estado_servo_flexionn1);

  Serial.print(" - goto_servo_flexionn2: ");
  Serial.println(goto_servo_flexionn2);

}

void mover_servo_flexionn1(int pos) {
  goto_servo_flexionn1 = pos;
}

void mover_servo_flexionn2(int pos) {
  goto_servo_flexionn2 = pos;
}

void mover_servo_Lampara(int pos) {
  goto_servo_Lampara = pos;
}

void mover_servo_base(int pos) {
  goto_servo_base = pos;
}*/
