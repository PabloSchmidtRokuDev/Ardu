/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

// #include <Servo.h>

// Servo myservo1;  // create servo object to control a servo
// Servo myservo2;
// Servo myservo3;
// Servo myservo4;
// Servo myservo5;



#define pin_Fijo 2
#define pin_suave 0
#define pin_flash 4

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("iniciando....");



  pinMode(pin_Fijo, OUTPUT);
  pinMode(pin_suave, OUTPUT);
  pinMode(pin_flash, OUTPUT);

  analogWrite(pin_Fijo, 200);

  analogWrite(pin_suave, 0);
  analogWrite(pin_flash, 0);
}

int i = 5;
bool flag = true;
bool flash = true;

void loop() {

  Serial.print("i: ");
  Serial.print(i % 2); 
  Serial.print("\t ");
  Serial.println(i);

  if (i <= 6) {
    flag = true;
    delay(1000);
  } else if (i >= 254) {
    flag = false;
    delay(1000);
  }

  delay(100);

  analogWrite(pin_suave, i);

  if (i % 4 == 0) {
    if (flash) analogWrite(pin_flash, 0);
    else analogWrite(pin_flash, 255);
    flash = !flash;
  }

  if (flag) {
    i++;
    // i++;
    // i++;
  } else {
    i--;
    // i--;
    // i--;
  }
}
