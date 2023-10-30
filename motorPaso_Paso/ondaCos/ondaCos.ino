#include "FastLED.h"
#include <Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

#define NUM_LEDS1 65 * 6 + 15
#define NUM_LEDS2 336
#define NUM_LEDS3 570

#define NUM_LEDS NUM_LEDS1

#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;
CRGB leds[NUM_LEDS];


#define PIN_LED_DATA 12

int servoPin1 = 13;
int servoPin2 = 27;
int servoPin3 = 26;
int servoPin4 = 25;
int servoPin5 = 33;
int servoPin6 = 32;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);
  myservo4.attach(servoPin4);
  myservo5.attach(servoPin5);
  myservo6.attach(servoPin6);

  myservo1.write(0);
  myservo2.write(0);
  myservo3.write(0);
  myservo4.write(0);
  myservo5.write(0);
  myservo6.write(0);

delay(5000);
  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


  for (int i = 0; i <= NUM_LEDS; i++) leds[i] = CRGB::Black;
  FastLED.show();

  Serial.println("start");
}

String SerialData = "";

// float max = 6.3;
// float cont = 0;
int maxP = 100;
void loop() {
  // put your main code here, to run repeatedly:
  //=(cos(C1)+1)/2
  //=1*(180-100)*D1+100
  //cont += 0.1;
  for (int i = 0; i <= maxP; i++) {
    myservo1.write(i);
    myservo2.write(i);
    myservo3.write(i);
    myservo4.write(i);
    myservo5.write(i);
    myservo6.write(i);
    delay(50);
    Serial.print(">");
    leds[i] = CRGB::Gold;
    FastLED.show();
  }
Serial.println("");
  for (int i = 0; i <= maxP; i++) {
    myservo1.write(maxP - i);
    myservo2.write(maxP - i);
    myservo3.write(maxP - i);
    myservo4.write(maxP - i);
    myservo5.write(maxP - i);
    myservo6.write(maxP - i);
    delay(50);
    Serial.print("<");
    leds[maxP -i] = CRGB::Black;
    FastLED.show();
  }
Serial.println("");
  // float cos_result = (cos(cont) + 1) / 2;
  // float posServo = 1 * (180 - 100) * cos_result + 100;
  // int posEntero = posServo;
  // Serial.println(posEntero);
  // delay(100);
  // if (cont == max) cont = 0;
  // if (Serial.available() > 0) {
  //   char c = Serial.read();
  //   SerialData += c;

  //   if (c == '\n') {
  //     Serial.print(SerialData);
  //     SerialData = "";
  //   }
  // }
}
