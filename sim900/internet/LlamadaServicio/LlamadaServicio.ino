#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library

const int pinGPSrx = 26;
const int pinGPStx = 28;

const int pinSIM900rx = 53;
const int pinSIM900tx = 52;

const int pinEncennderSIM900 = 47;

/* Create object named SIM900 of the class SoftwareSerial */
SoftwareSerial SIM900(pinSIM900rx, pinSIM900tx);

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
TinyGPS gps;
SoftwareSerial ssGPS(pinGPSrx, pinGPStx);

MCUFRIEND_kbv tft;

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410

char incoming_char = 0;

void setup() {

  pinMode(pinEncennderSIM900, OUTPUT);
  
  SIM900.begin(9600);  /* Define baud rate for software serial communication */
  Serial.begin(9600); /* Define baud rate for serial communication */
  Serial1.begin(9600);

  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
  tft.begin(ID);
  tft.setRotation(0);

  tft.fillScreen(GREY);
  addText("iniciando...");
  addText("Encendiendo Sim900");
  
  digitalWrite(pinEncennderSIM900, HIGH);
  delay(1000);
  digitalWrite(pinEncennderSIM900, LOW);
  
  delay(20000);  
  addText("AT");
  SIM900.println("AT"); /* Check Communication */
  ShowSerialData();

  delay(1000);
  conectarInternet();
}

void loop() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

    if (flat != TinyGPS::GPS_INVALID_F_ANGLE){
      if (flon != TinyGPS::GPS_INVALID_F_ANGLE){
        sendLocation(String(flat, 6), String(flon, 6));
      }
    }
    String valor;

    valor = "LAT=" + String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
 
    valor = valor + " - LON=" + String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

    
    valor = valor +  " - SAT=" + String(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    valor = valor + " PREC=" +  String(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    addText(valor);
  }
  
  gps.stats(&chars, &sentences, &failed);
  String valor = " CHARS=";
  valor = valor +  String(chars);
  valor = valor +" SENTENCES=";
  valor = valor +String(sentences);
  valor = valor +" CSUM ERR=";
  valor = valor + String(failed);

  addText(valor);
  if (chars == 0)
    addText("** No characters received from GPS: check wiring **");

    delay(5000);
}

void conectarInternet()
{
addText("Iniciando conexion a internet");
  /* Configure bearer profile 1 */
  //addText("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");    
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
  delay(500);
  ShowSerialData();

  
  //addText("AT+SAPBR=3,1,\"APN\",\"TATA.DOCOMO.INTERNET\"");  
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"TATA.DOCOMO.INTERNET\"");  /* APN of the provider */
  delay(500);
  ShowSerialData();

  addText("AT+SAPBR=1,1");
  SIM900.println("AT+SAPBR=1,1"); /* Open GPRS context */
  delay(500);
  ShowSerialData();
  
  addText("AT+SAPBR=2,1");
  SIM900.println("AT+SAPBR=2,1"); /* Query the GPRS context */
  delay(500);
  ShowSerialData();

  //addText("AT+HTTPINIT");
  SIM900.println("AT+HTTPINIT"); /* Initialize HTTP service */
  delay(500); 
  ShowSerialData();

  //Serial.print("AT+HTTPPARA=\"CID\",1");
  SIM900.println("AT+HTTPPARA=\"CID\",1");  /* Set parameters for HTTP session */
  delay(500);
  ShowSerialData();
  addText("fin. si todo  esta ok!");
}

void sendLocation(String Lat, String Lon){
  String valor = "";//"AT+HTTPPARA=\"URL\",\"http://devpablo.com/saveLL.php?Lat=" + Lat + "&Lon=" + Lon;
  addText("Enviando coordenadas...");
  //addText(valor);
  SIM900.println("AT+HTTPPARA=\"URL\",\"http://devpablo.com/saveLL.php?Lat=" + Lat + "&Lon=" + Lon + "\"");  /* Set parameters for HTTP session */
  delay(500);
  ShowSerialData();

  //addText("AT+HTTPACTION=0 - Start GET session");
  SIM900.println("AT+HTTPACTION=0");  /* Start GET session */
  delay(500);
  ShowSerialData();

  addText("AT+HTTPREAD - Read data from HTTP server");
  SIM900.println("AT+HTTPREAD");  /* Read data from HTTP server */
  delay(5000);
  ShowSerialData();

   addText("Envio completo...");
}

void cerrarConecxion(){
  addText("AT+HTTPTERM");  
  SIM900.println("AT+HTTPTERM");  /* Terminate HTTP service */
  delay(500);
  ShowSerialData();
  
  addText("AT+SAPBR=0,1");
  SIM900.println("AT+SAPBR=0,1"); /* Close GPRS context */
  delay(500);
  ShowSerialData();
}

bool ShowSerialData()
{
  int count=0; 
  unsigned char buffer[64]; //port

  for (int i=0; i<count;i++){
    buffer[i]=NULL;                  // clear all index of array with command NULL
  }
  
  while(SIM900.available()){          // reading data into char array 
      buffer[count++]=SIM900.read();     // writing data into array
      if(count == 64)break;
    }

   // Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    String linea = (char*)buffer;
    linea.trim();
    
//Serial.println("sim900: " + linea);
  addText("sim900: " + linea);
  if (linea == "OK") return true; else return false;
    
}


int yActual = 20;
int conLineas = 0;

void addText(String msg){
  msg.trim();
  if (msg != ""){

  int  r=0, t=0;
  String oneLine;
  
  int salto = msg.indexOf("\r\n");
  
  while (salto > 0)
  { 
 
    oneLine = msg.substring(r, salto); 
    msg.replace(oneLine, "");
    msg.trim();
    r=(salto+1); 
    t++; 
    sendMSG(oneLine);

    salto = msg.indexOf("\r\n");

    }
    
    sendMSG(msg);
}
}
void sendMSG(String msg){
      msg = String(conLineas) + "- " + msg;
      
      Serial.println(msg);
      int x = 10;
       
      int sz = 1;
      
      showmsgXY(x, yActual, sz, &FreeSans9pt7b, msg.c_str());
      yActual = yActual + 35;
    
      if (conLineas >= 7){
        yActual = 20;
        conLineas = 0;
        tft.fillScreen(WHITE);
      }
      else
      {
        conLineas++;
      }
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y + 1, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(BLACK);
    tft.setTextSize(sz);
    tft.print(msg);
    delay(500);
}
