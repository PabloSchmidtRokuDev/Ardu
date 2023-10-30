/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <Arduino.h>

#define SERVER_IP "http://devpablo.com/Home/Estados.php"


void setup() {

  Serial.begin(115200);
Serial2.begin(9600);

pinMode(27, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.println();

Serial.println("arracando sim900");  
    digitalWrite(27, HIGH);
    delay(1000); //Retardo para que encuentra a una RED
    digitalWrite(27, LOW);
delay(10000);
  Serial2.print("AT");
  Serial.println("ok");  
}

String header = "";
bool lineComplete = false;
String serialStr = "";

void loop() {


  if (Serial.available())  //Si hay caracteres en puerto serial por hardware
  {
    char c = Serial.read();  //leer carecteres en puerto serial por hardware

    if (c = '\n') {
      Serial2.println(serialStr);  //Mostras los caracteres obtenidos
      serialStr = "";
    } else {
      serialStr += c;
    }
  }


  while (Serial2.available() )//& lineComplete == false)  //si hay caracteres disponibles en el puerto serial por software
  {
    char caracter = Serial2.read();  //leer carecteres en puerto serial por software
    Serial.write(caracter);         // print it out the serial monitor

    // header += caracter;
    // if (caracter == '\n') {  // if the byte is a newline character
    //   lineComplete = true;
    //   header.trim();
    //   //Serial.println(">" + header + "<");
    //   if (header == "Call Ready") {
    //     Serial.println("modo_recibe_mensaje...");
    //     modo_recibe_mensaje();
    //   }
    // }
  }


}

void modo_recibe_mensaje() {
  //Configura el modo texto para enviar o recibir mensajes
  Serial2.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);

  Serial2.print("AT+CNMI=2,2,0,0,0\r");

  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(1000);
}