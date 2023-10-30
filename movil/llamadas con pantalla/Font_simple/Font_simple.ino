#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library

#include <SoftwareSerial.h>;
MCUFRIEND_kbv tft;

SoftwareSerial SIM900(53, 51); // Configura el puerto serial para el SIM GSM

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410

char incoming_char = 0; //Variable que guarda los caracteres que envia el SIM GSM
int salir = 0;

void setup(void)
{

    uint16_t ID = tft.readID();
    
  Serial.begin(9600); //Configura velocidad serial para el Arduino
   //Mensaje OK en el arduino, para saber que todo va bien.

  
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(0);
    
  tft.fillScreen(BLACK);
  addText("iniciando...");
  addText("Esperando conexion");
  SIM900.begin(9600); //Configura velocidad serial para el SIM
  delay(5000); //Retardo para que encuentra a una RED
  addText("entra loop");
  SIM900.print("AT");
}

char caracter;

void loop(void)
{



   //Envíamos y recibimos datos
 /*if (Serial.available() > 0){
  char *msg = Serial.read();
  addText(msg);
  SIM900.write(msg);
 }
 
 
 if (SIM900.available() > 0){
  addText(SIM900.read());
   Serial.write(SIM900.read());
 }

  addText("llamando...");
      llamar(); //Llama
      //addText("iniciando...");
      
 // mensaje_sms(); //Envia mensaje
  addText("esperando mensajes");
  modo_recibe_mensaje();
  for (;;) {
    if (SIM900.available() > 0) {

      incoming_char = SIM900.read(); //Get the character from the cellular serial port.
      addText(incoming_char); //Print the incoming character to the terminal.
      Serial.print(incoming_char); //Print the incoming character to the terminal.
    }
    if (Serial.available() > 0) {
      if (Serial.read() == 'A') break;
    }
  }
  addText("OK-2");

  delay(100);
  SIM900.println();
  delay(30000);
  */
  if (SIM900.available())//si hay caracteres disponibles en el puerto serial por software
  {
    caracter = SIM900.read();//leer carecteres en puerto serial por software
    addText(caracter);//Mostras los caracteres obtenidos
  }
  
  if (Serial.available())//Si hay caracteres en puerto serial por hardware
  {
    caracter = Serial.read();//leer carecteres en puerto serial por hardware
    addText("RECIBO: " + caracter);
    SIM900.print(caracter);//Mostras los caracteres obtenidos
  }

    /*showmsgXY(20, 10, 1, NULL, "System x1");
    showmsgXY(20, 24, 2, NULL, "System x2");
    showmsgXY(20, 60, 1, &FreeSans9pt7b, "FreeSans9pt7b");
    showmsgXY(20, 80, 1, &FreeSans12pt7b, "FreeSans12pt7b");
    showmsgXY(20, 100, 1, &FreeSerif12pt7b, "FreeSerif12pt7b");
    showmsgXY(20, 120, 1, &FreeSmallFont, "FreeSmallFont");
    showmsgXY(5, 180, 1, &FreeSevenSegNumFont, "01234");
    showmsgXY(5, 190, 1, NULL, "System Font is drawn from topline");
    tft.setTextColor(RED, GREY);
    tft.setTextSize(2);
    tft.setCursor(0, 220);
    tft.print("7x5 can overwrite");
    delay(1000);
    tft.setCursor(0, 220);
    tft.print("if background set");
    delay(1000);
    showmsgXY(5, 260, 1, &FreeSans9pt7b, "Free Fonts from baseline");
    showmsgXY(5, 285, 1, &FreeSans9pt7b, "Free Fonts transparent");
    delay(1000);
    showmsgXY(5, 285, 1, &FreeSans9pt7b, "Free Fonts XXX");
    delay(1000);
    showmsgXY(5, 310, 1, &FreeSans9pt7b, "erase backgnd with fillRect()");
    delay(10000);
    */
}

int yActual = 17;

void addText(const char *msg){
  Serial.print(msg);
  int x = 20;
   
  int sz = 1;
  
  showmsgXY(x, yActual, sz, NULL, msg);
  yActual = yActual + 17;

  if (yActual > 238){
    yActual = 17;
    tft.fillScreen(RED);
  }
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y + 1, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
    delay(1000);
}



void llamar()
// Función que permite llamar a un celular local
{
  SIM900.println("ATD 34679685164;"); //Celular
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
  SIM900.println("AT+CMGS=\"34679685164\""); // recipient's mobile number, in international format
  delay(100);
  SIM900.println("Saludos desde HetPro"); // message to send
  delay(100);
  SIM900.println((char) 26); // End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
  delay(100);
  SIM900.println();
  delay(5000); // Tiempo para que se envie el mensaje
  addText("SMS sent successfully");
}

void espera_mensaje() {
  salir = 1;
  const char* msjj="";
  while (salir == 1) {
    if (SIM900.available() > 0) {
      incoming_char = SIM900.read(); //Get the character from the cellular serial port.
      msjj = msjj +  incoming_char;
      
      salir = 0;
    }
  }
  addTe
  689 732 464
  
  xt(msjj); //Print the incoming character to the terminal.
}
void modo_recibe_mensaje() {
  //Configura el modo texto para enviar o recibir mensajes
  SIM900.print("AT+CMGF=1\r"); // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");

  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(1000);
}
