
#include <Arduino.h>

//#include "PinDefinitionsAndMore.h"  // Define macros for input and output pin etc.

#include <ezBuzzer.h>  // ezBuzzer library

ezBuzzer buzzer(5);  // create ezBuzzer object that attach to a pin;

#define RAW_BUFFER_LENGTH 180

#define MARK_EXCESS_MICROS 20  // Adapt it to your IR receiver module. 20 is recommended for the cheap VS1838 modules.

#include <IRremote.hpp>
int melodya1[] = { NOTE_E5, NOTE_E5, NOTE_E5 };
int melodya2[] = { NOTE_E5, NOTE_E5, NOTE_E5 };
int melodya3[] = { NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5 };
int melodya4[] = { NOTE_E5 };
// int melodya5[] = { NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5 };
// int melodya6[] = { NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5 };
// int melodya7[] = { NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5 };
// int melodya8[] = { NOTE_D5, NOTE_G5 };


// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations1[] = { 8, 8, 4 };
int noteDurations2[] = { 8, 8, 4 };
int noteDurations3[] = { 8, 8, 8, 8 };
int noteDurations4[] = { 2 };
// int noteDurations5[] = { 8, 8, 8, 8 };
// int noteDurations6[] = { 8, 8, 8, 16, 16 };
// int noteDurations7[] = { 8, 8, 8, 8 };
// int noteDurations8[] = { 4, 4 };

void ejecutarSonido(int numSon) {
  buzzer.stop();
  if (numSon == 0) {
    Serial.println("----son B----");
    buzzer.beep(100);
  }
  if (numSon == 1) {
    Serial.println("----son 1----");
    buzzer.playMelody(melodya1, noteDurations1, sizeof(noteDurations1) / sizeof(int));
  }
  if (numSon == 2) {
    Serial.println("----son 2----");
    buzzer.playMelody(melodya2, noteDurations2, sizeof(noteDurations2) / sizeof(int));
  }
  if (numSon == 3) {
    Serial.println("----son 3----");
    buzzer.playMelody(melodya3, noteDurations3, sizeof(noteDurations3) / sizeof(int));
  }
  // if (numSon == 4){Serial.println("----son 4----"); buzzer.playMelody(melodya4, noteDurations4, sizeof(noteDurations4) / sizeof(int));}
  // if (numSon == 5){Serial.println("----son 5----"); buzzer.playMelody(melodya5, noteDurations5, sizeof(noteDurations5) / sizeof(int));}
  // if (numSon == 6){Serial.println("----son 6----"); buzzer.playMelody(melodya6, noteDurations6, sizeof(noteDurations6) / sizeof(int));}
  // if (numSon == 7){Serial.println("----son 7----"); buzzer.playMelody(melodya7, noteDurations7, sizeof(noteDurations7) / sizeof(int));}
  // if (numSon == 8){Serial.println("----son 8----"); buzzer.playMelody(melodya8, noteDurations8, sizeof(noteDurations8) / sizeof(int));}
}

void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);  // Status message will be sent to PC at 9600 baud
  IrReceiver.begin(8);

  // Serial.print(F("Ready to receive IR signals of protocols: "));
  // printActiveIRProtocols(&Serial);
  // Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));

  // // infos for receive
  // Serial.print(RECORD_GAP_MICROS);
  // Serial.println(F(" us is the (minimum) gap, after which the start of a new IR packet is assumed"));
  // Serial.print(MARK_EXCESS_MICROS);
  // Serial.println();
  // Serial.println(F("Because of the verbose output (>200 ms at 115200), repeats are probably not dumped correctly!"));
  // Serial.println();
}

//+=============================================================================
// The repeating section of the code
int c = 0;
void loop() {

  buzzer.loop();
  if (IrReceiver.decode()) {  // Grab an IR code
    // At 115200 baud, printing takes 200 ms for NEC protocol and 70 ms for NEC repeat
    Serial.println("------------------------------------");  // blank line between entries
    Serial.println();                                        // 2 blank lines between entries

    IrReceiver.printIRResultShort(&Serial);
    // Check if the buffer overflowed
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
      // Serial.println(F("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__));
      // see also https://github.com/Arduino-IRremote/Arduino-IRremote#compile-options--macros-for-this-library
    } else {
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      }

      Serial.print("value:");
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      Serial.println("--------");  // blank line between entries

      Serial.println();
      ejecutarSonido(c);
      c++;
      if (c == 5) c = 0;
      // buzzer.begin(0);

      // buzzer.distortion(NOTE_C3, NOTE_C5);
      // buzzer.distortion(NOTE_C5, NOTE_C3);

      // buzzer.end(100);
      //IrReceiver.printIRSendUsage(&Serial);
      // Serial.println();
      // Serial.println(F("Raw result in internal ticks (50 us) - with leading gap"));
      // IrReceiver.printIRResultRawFormatted(&Serial, false); // Output the results in RAW format
      // Serial.println(F("Raw result in microseconds - with leading gap"));
      // IrReceiver.printIRResultRawFormatted(&Serial, true);  // Output the results in RAW format
      // Serial.println();                               // blank line between entries
      // Serial.print(F("Result as internal 8bit ticks (50 us) array - compensated with MARK_EXCESS_MICROS="));
      // Serial.println(MARK_EXCESS_MICROS);
      // IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, false); // Output the results as uint8_t source code array of ticks
      // Serial.print(F("Result as microseconds array - compensated with MARK_EXCESS_MICROS="));
      // Serial.println(MARK_EXCESS_MICROS);
      // IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, true); // Output the results as uint16_t source code array of micros
      // IrReceiver.printIRResultAsCVariables(&Serial);  // Output address and data as source code variables

      //IrReceiver.compensateAndPrintIRResultAsPronto(&Serial);

      /*
             * Example for using the compensateAndStorePronto() function.
             * Creating this String requires 2210 bytes program memory and 10 bytes RAM for the String class.
             * The String object itself requires additional 440 bytes RAM from the heap.
             * This values are for an Arduino Uno.
             */
      //        Serial.println();                                     // blank line between entries
      //        String ProntoHEX = F("Pronto HEX contains: ");        // Assign string to ProtoHex string object
      //        if (int size = IrReceiver.compensateAndStorePronto(&ProntoHEX)) {   // Dump the content of the IReceiver Pronto HEX to the String object
      //            // Append compensateAndStorePronto() size information to the String object (requires 50 bytes heap)
      //            ProntoHEX += F("\r\nProntoHEX is ");              // Add codes size information to the String object
      //            ProntoHEX += size;
      //            ProntoHEX += F(" characters long and contains "); // Add codes count information to the String object
      //            ProntoHEX += size / 5;
      //            ProntoHEX += F(" codes");
      //            Serial.println(ProntoHEX.c_str());                // Print to the serial console the whole String object
      //            Serial.println();                                 // blank line between entries
      //        }
    }
    IrReceiver.resume();  // Prepare for the next value
  }
}
