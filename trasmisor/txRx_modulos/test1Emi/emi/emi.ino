#include <VirtualWire.h>

const int dataPin = 13;

void setup()
{
    Serial.begin(115200);     
    vw_setup(2000);
    vw_set_tx_pin(dataPin);
}



String msg = "Hola mundo";
void loop()
{
 
    
msg = "eje1: ";

   Serial.println(msg); 
    vw_send((uint8_t *)msg.c_str(), strlen(msg.c_str()));
    vw_wait_tx();
    
    delay(200);
}
