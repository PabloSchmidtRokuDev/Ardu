/*
Herramientas Tecnologicas Profesionales
www.hetpro-store.com

Tutorial Original:
https://tronixstuff.com/2014/01/08/tutorial-arduino-and-sim900-gsm-modules/

Comandos AT:
https://probots.co.in/Manuals/SIM900%20GSM%20Modem%20-%20Starter%20Guide.pdf

Hoja de datos:

Instrucciones para Arduino: Configura la terminal serial a 19200 baudios para que puedas ver los mensajes.
*/

#include <SoftwareSerial.h>;

SoftwareSerial SIM900(7, 8); // Configura el puerto serial para el SIM GSM

char incoming_char = 0; //Variable que guarda los caracteres que envia el SIM GSM
int salir = 0;
void setup() {
  SIM900.begin(19200); //Configura velocidad serial para el SIM
  delay(25000); //Retardo para que encuentra a una RED
  Serial.begin(19200); //Configura velocidad serial para el Arduino
  Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.
}

void llamar()
// Función que permite llamar a un celular local
{
  SIM900.println("ATD 33XXXXXXXX;"); //Celular
  delay(100);
  SIM900.println();
  delay(30000); // wait for 30 seconds...
  SIM900.println("ATH"); // Cuelta el telefono
  delay(1000);
}
void mensaje_sms()
//Funcion para mandar mensaje de texto
{

  SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
  delay(100);
  SIM900.println("AT+CMGS=\"33XXXXXXXX\""); // recipient's mobile number, in international format
  delay(100);
  SIM900.println("Saludos desde HetPro"); // message to send
  delay(100);
  SIM900.println((char) 26); // End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
  delay(100);
  SIM900.println();
  delay(5000); // Tiempo para que se envie el mensaje
  Serial.println("SMS sent successfully");
}

void espera_mensaje() {
  salir = 1;
  while (salir == 1) {
    if (SIM900.available() > 0) {
      incoming_char = SIM900.read(); //Get the character from the cellular serial port.
      Serial.print(incoming_char); //Print the incoming character to the terminal.
      salir = 0;
    }
  }
}
void modo_recibe_mensaje() {
  //Configura el modo texto para enviar o recibir mensajes
  SIM900.print("AT+CMGF=1\r"); // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");

  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(1000);
}
void loop() {
  llamar(); //Llama
  mensaje_sms(); //Envia mensaje
  modo_recibe_mensaje();
  for (;;) {
    if (SIM900.available() > 0) {

      incoming_char = SIM900.read(); //Get the character from the cellular serial port.
      Serial.print(incoming_char); //Print the incoming character to the terminal.
    }
    if (Serial.available() > 0) {
      if (Serial.read() == 'A') break;
    }
  }
  Serial.println("OK-2");

  delay(100);
  SIM900.println();
  delay(30000);
  while (1); // Espérate por tiempo indefinido

}
