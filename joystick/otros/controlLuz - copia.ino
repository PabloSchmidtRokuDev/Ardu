
#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <Servo.h>

unsigned long  tiemploParaFlash = 10000;
unsigned long  tiempoParaInactividad = 10000;//*5; //1 seg * 60 seg * 5 = 5 min

unsigned long tiempoFlash = 0;
unsigned long tiempoInactividad = 0;
unsigned long tiempoContador = 0;
unsigned long tiempoInicial = 0;

#define SERVO_PIN1 2
#define SERVO_PIN2  0
#define PHOTO_PIN  4

#define SERVO_PIN1_DEF_POS  90
#define SERVO_PIN2_DEF_POS  90


#ifndef STASSID
const char* ssid     = "MOVISTAR_CAE0";
const char* password = "aTchezHwfCpu8MLk6HNn";
#endif

WiFiServer server(80);

String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 140);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional


enum DN
{
  GO_ARRIBA, //go forward
  GO_LEFT, //left turn
  GO_RIGHT,//right turn
  GO_ABAJO,//backward
  STOP_STOP,
  VERIFICACION_ACCION,
  RAFAGA_FOTOS,
  TOMAR_1_FOTO,
  AGREGAR_DISTANCIA,
  QUITAR_DISTANCIA,
  POWER_PRESSED,
  DEF
}

Drive_Num = DEF;
DN Last_Drive_Num = DEF;
int luzEstado = 0;


int distance1 = SERVO_PIN1_DEF_POS;
int servo1PosActual = SERVO_PIN1_DEF_POS;
Servo servo1;

int distance2 = SERVO_PIN2_DEF_POS;
int servo2PosActual = SERVO_PIN1_DEF_POS;
Servo servo2;

int incrementos = 4;
boolean habilitado = false;

void setup() {
  Serial.begin(115200);

  pinMode(PHOTO_PIN, OUTPUT);

  servo1.attach(SERVO_PIN1);
  servo1.write(SERVO_PIN1_DEF_POS);

  servo2.attach(SERVO_PIN2);
  servo2.write(SERVO_PIN2_DEF_POS);
    
    Serial.print("Connecting to ");
  Serial.println(ssid);

    // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
 

}

int lectura = 0;
unsigned long suma;
unsigned long dif;

int ccc =0;
void loop() {
  WiFiClient clientS = server.available(); // Listen for incoming clients

  if (clientS)
  { // If a new client connects,
    //Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (clientS.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (clientS.available())
      { // if there's bytes to read from the client,
        char c = clientS.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor

        header += c;
        if (c == '\n')
        {
         

            clientS.println("HTTP/1.1 200 OK");
            clientS.println("Content-type:text/html");
            clientS.println("Connection: close");
            clientS.println('\r');

            Serial.print(header);

            if (ccc == 10){
              Serial.println("");      
              ccc = -1;        
            }
            ccc++;
   
            break;
          }

        }

      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    clientS.stop();
  }
  

  
  setServoPosition();

  if (habilitado == true && Drive_Num != DEF ) {

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
    }
}

void encenderLuz() {
  Serial.println("funcion tomarUnaFoto ");
  if (luzEstado == 0)
  {
    digitalWrite(PHOTO_PIN, HIGH);
    luzEstado = 1;
  } else
  {
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

