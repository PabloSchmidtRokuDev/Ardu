#include <ESP8266WiFi.h>

#include "config.h"  // Sustituir con datos de vuestra red

#include <Servo.h>

#define SERVO_PIN1 2
#define SERVO_PIN2 0
#define PHOTO_PIN 4

#define SERVO_PIN1_DEF_POS 90
#define SERVO_PIN2_DEF_POS 90

int luzEstado = 0;


int distance1 = SERVO_PIN1_DEF_POS;
int servo1PosActual = SERVO_PIN1_DEF_POS;
Servo servo1;

int distance2 = SERVO_PIN2_DEF_POS;
int servo2PosActual = SERVO_PIN1_DEF_POS;
Servo servo2;

int incrementos = 4;
boolean habilitado = false;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(PHOTO_PIN, OUTPUT);

  servo1.attach(SERVO_PIN1);
  servo1.write(SERVO_PIN1_DEF_POS);

  servo2.attach(SERVO_PIN2);
  servo2.write(SERVO_PIN2_DEF_POS);

  Serial.print("Connecting to ");

  while(!WiFi.softAP(ssid, password))
   {
     Serial.println(".");
     delay(100);
   }
    WiFi.softAPConfig(ip, gateway, subnet);
    
  Serial.println("");
   Serial.print("Iniciado AP:\t");
   Serial.println(ssid);
   Serial.print("IP address:\t");
   Serial.println(WiFi.softAPIP());
  server.begin();
}

int lectura = 0;
unsigned long suma;
unsigned long dif;
String header;

void loop() {
 WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            
              Serial.println(header);
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
          
           
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }



  //server.handleClient();



  /* setServoPosition();

  if (habilitado == true) {

      //tiempoInactividad = tiempoContador + tiempoParaInactividad;

      if (Drive_Num == POWER_PRESSED) {
          digitalWrite(PHOTO_PIN, LOW);
          luzEstado = 0;

          distance1 = SERVO_PIN1_DEF_POS;
          servo1PosActual = SERVO_PIN1_DEF_POS;
          distance2 = SERVO_PIN2_DEF_POS;
          servo2PosActual = SERVO_PIN1_DEF_POS;
          
          servo1.write(distance1);
          servo2.write(distance2);
        
      } 
      else  
      if ((Drive_Num == GO_RIGHT) || (Drive_Num == GO_LEFT) || (Drive_Num == GO_ARRIBA) || (Drive_Num == GO_ABAJO))
      {
        Last_Drive_Num = Drive_Num;
        if (Drive_Num == GO_RIGHT) distance1 += incrementos;
        if (Drive_Num == GO_LEFT) distance1 -= incrementos;
        if (Drive_Num == GO_ARRIBA) distance2 += incrementos;
        if (Drive_Num == GO_ABAJO) distance2 -= incrementos;


        if (distance1 > 180) distance1 = 180;
        if (distance1 < 0) distance1 = 0;

        if (distance2 > 180) distance2 = 180;
        if (distance2 < 0) distance2 = 0;
  
      }


      Drive_Num = DEF;
    }*/
}

void encenderLuz() {
  Serial.println("funcion tomarUnaFoto ");
  if (luzEstado == 0) {
    digitalWrite(PHOTO_PIN, HIGH);
    luzEstado = 1;
  } else {
    digitalWrite(PHOTO_PIN, LOW);
    luzEstado = 0;
  }
}

void setServoPosition() {

  if (distance1 != servo1PosActual || distance2 != servo2PosActual) {

    if (distance1 != servo1PosActual && distance1 > servo1PosActual) servo1PosActual++;
    if (distance1 != servo1PosActual && distance1 < servo1PosActual) servo1PosActual--;

    if (distance2 != servo2PosActual && distance2 > servo2PosActual) servo2PosActual++;
    if (distance2 != servo2PosActual && distance2 < servo2PosActual) servo2PosActual--;

    servo1.write(servo1PosActual);
    servo2.write(servo2PosActual);
  }
}