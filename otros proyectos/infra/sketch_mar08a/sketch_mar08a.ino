#include <IRremote.h>
int input_pin = 8;  //connect S12 in extension shield(D12 in Arduino) to IR receiver S pin
IRrecv irrecv(input_pin);
decode_results signals;
#define DECODE_LG


#define IR_ADVANCE 0x00FF18E7  //code from IR controller "▲" button
#define IR_BACK 0x00FF4AB5     //code from IR controller "▼" button

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // enable input from IR receiver

  pinMode(4, OUTPUT);
  digitalWrite(4, false);
}
void loop() {
  if (irrecv.decode(&signals)) {
    Serial.print("value:");
    Serial.println(signals.value, HEX);

    Serial.print("\t decode_type:");
    Serial.print(signals.decode_type);

    Serial.print("\t bits:");
    Serial.print(signals.bits);

    Serial.print("\t magnitude:");
    Serial.print(signals.magnitude);

    Serial.print("\t isRepeat:");
    Serial.print(signals.isRepeat);

    Serial.print("\t rawlen:");
    Serial.print(signals.rawlen);

    Serial.print("\t overflow:");
    Serial.println(signals.overflow);



    irrecv.resume();  // get the next signal
  }
}
