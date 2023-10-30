#include <VirtualWire.h>

const int dataPin = 2;

void setup()
{
  Serial.begin(115200);     
    vw_setup(2000);
    vw_set_rx_pin(dataPin);
    vw_rx_start();
Serial.print("start: ");
}

void loop()
{
  

   // uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buf[VW_MAX_MESSAGE_LEN] = {};
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    if (vw_get_message(buf, &buflen)) 
    {
   
      Serial.print("Mensaje: ");
      for (int i = 0; i < buflen; i++)
      {
         Serial.print((char)buf[i]);
      }
      Serial.println("");
    }
}
