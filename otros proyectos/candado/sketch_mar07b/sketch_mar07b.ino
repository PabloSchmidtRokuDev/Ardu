
#include <VirtualWire.h>
 
const int dataPin = 3;
const int ledPin = 4;
 
void setup()
{
    vw_setup(2000);
    vw_set_rx_pin(dataPin);
    vw_rx_start();

 Serial.begin(9600);
    Serial.println("iniciando");

    
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, false);
    delay(2000);
    digitalWrite(ledPin, true);
}
 
void loop()
{
    uint8_t data;
    uint8_t dataLength=1;
 Serial.println("leyendo");
    if (vw_get_message(&data,&dataLength))
    {
      Serial.println("codigo");
        Serial.println(data);
        Serial.println(dataLength);
Serial.println("******************");
        
        if((char)data=='a')
        {
            digitalWrite(ledPin, true);
        }
        else if((char)data=='b')
        {
            digitalWrite(ledPin, false);
        }            
    }
}
