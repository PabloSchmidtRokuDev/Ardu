#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define MIN_PULSE_WIDTH 544
#define MAX_PULSE_WIDTH 2400
#define FREQUENCY 200

void setup() {
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);  // This is the maximum PWM frequency

}

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide =map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

void loop() {

//     for (uint16_t i=0; i<4096; i += 8) {
//     for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
//       pwm.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
//     }
// #ifdef ESP8266
//     yield();  // take a breather, required for ESP8266
// #endif
//     }
    delay(3000);
   
pwm.setPWM(14, 0, pulseWidth(180));
pwm.setPWM(15, 0, pulseWidth(180));
    

pwm.setPWM(12, 0, pulseWidth(150));
  delay(3000);


pwm.setPWM(14, 0, pulseWidth(0));
pwm.setPWM(15, 0, pulseWidth(0));

pwm.setPWM(12, 0, 2);

  
  // pwm.setPWM(0, 0, pulseWidth(0));
  // pwm.setPWM(1, 0, pulseWidth(180));
  // delay(1000);
  // pwm.setPWM(4, 0, pulseWidth(0));
  // delay(1000);
  // pwm.setPWM(0, 0, pulseWidth(180));
  // pwm.setPWM(1, 0, pulseWidth(90));
  // delay(500);
  // pwm.setPWM(4, 0, pulseWidth(180));
  // delay(1000);
  // pwm.setPWM(0, 0, pulseWidth(90));
  // pwm.setPWM(1, 0, pulseWidth(0));
  // delay(1000);
}