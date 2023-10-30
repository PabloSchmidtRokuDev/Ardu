#include "ESP8266.h"
#include <SoftwareSerial.h>

const char* SSID = "Daniel_Fusiger_ext";
const char* PASSWORD = "Esagataesmia";


SoftwareSerial softSerial(2, 3); // RX, TX
ESP8266 wifi(softSerial);

void setup(void)
{
   pinMode(LED_BUILTIN, OUTPUT);

   Serial.begin(9600);
   Serial.print("setup begin\r\n");
   
   wifi.restart();
   delay(500);
   if (wifi.setOprToStationSoftAP()) {
      Serial.print("to station + softap ok\r\n");
   }
   else {
      Serial.print("to station + softap err\r\n");
   }

   if (wifi.joinAP(SSID, PASSWORD)) {
      Serial.print("Join AP success\r\n");
      Serial.print("IP: ");
      Serial.println(wifi.getLocalIP().c_str());
   }
   else {
      Serial.print("Join AP failure\r\n");
   }

   if (wifi.enableMUX()) {
      Serial.print("multiple ok\r\n");
   }
   else {
      Serial.print("multiple err\r\n");
   }

   if (wifi.startTCPServer(80)) {
      Serial.print("start tcp server ok\r\n");
   }
   else {
      Serial.print("start tcp server err\r\n");
   }

   if (wifi.setTCPServerTimeout(20)) {
      Serial.print("set tcp server timout 20 seconds\r\n");
   }
   else {
      Serial.print("set tcp server timout err\r\n");
   }

   Serial.println("setup end\r\n");
}

#define wifiWrite(A) wifi.send(mux_id, (uint8_t*) A, sizeof(A) - 1);
void loop(void)
{
   uint8_t buffer[128] = { 0 };
   uint8_t mux_id;

   uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
   if (len > 0) {
      Serial.print("Received from: ");
      Serial.print(mux_id);
      Serial.print("\r\n");

      wifiWrite("HTTP/1.1 200 OK\r\nnContent-Type: /html\r\nConnection: close\r\n\r\n");
      wifiWrite("<html>\n<head>\n<title>Luis Llamas</title>\n</head>\n<body>");
      wifiWrite("<h2>Salidas digitales</h2>");
      wifiWrite("<button onClick=location.href='./?data=0'>ON</button>");
      wifiWrite("<button onClick=location.href='./?data=1'>OFF</button>");
      wifiWrite("</body></html>");

      Serial.println("Send finish");

      for (uint32_t i = 0; i < len; i++) {
         char c = (char)buffer[i];
         if (c == '?')
         {
            if ((char)buffer[i + 6] == '1')
            {
               digitalWrite(LED_BUILTIN, HIGH);
               Serial.println("LED ON");
            }
            else
            {
               digitalWrite(LED_BUILTIN, LOW);
               Serial.println("LED OFF");
            }

            break;
         }
      }
   }
}
