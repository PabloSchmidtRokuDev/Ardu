/*
Herramientas Tecnologicas Profesionales
www.hetpro-store.com
Tutorial Original:
https://tronixstuff.com/2014/01/08/tutorial-arduino-and-Serial2-gsm-modules/
Comandos AT:
https://probots.co.in/Manuals/Serial2%20GSM%20Modem%20-%20Starter%20Guide.pdf
Hoja de datos:
Instrucciones para Arduino: Configura la terminal serial a 19200 baudios para que puedas ver los mensajes.
*/
#include <Adafruit_GFX.h>    // Core graphics library

#include <MCUFRIEND_kbv.h>   // Hardware-specific library

#include <SoftwareSerial.h>;

#include <Fonts/FreeSans9pt7b.h>

#include <Fonts/FreeSans12pt7b.h>

#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

MCUFRIEND_kbv tft;



#define BLACK 0x0000
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define GREY 0x8410

#define PIN_PUERTA 49
#define PIN_ESCUCHA_PUERTA 31

#define PIN_ENCENDER_SIM 51

SoftwareSerial sim900Serial(50, 52); // Configura el puerto serial para el SIM GSM

int salir = 0;

void setup() {
    Serial.begin(19200);
    //ESP32Serial.begin(9600);
    sim900Serial.begin(9600);

    pinMode(PIN_PUERTA, OUTPUT);
    pinMode(PIN_ENCENDER_SIM, OUTPUT);

    uint16_t ID = tft.readID();

    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(0);

    tft.fillScreen(BLACK);
    addText("iniciando... ISP");
    addText("Iniciando SIM900");


    digitalWrite(PIN_ENCENDER_SIM, HIGH);
    delay(1000); //Retardo para que encuentra a una RED
    digitalWrite(PIN_ENCENDER_SIM, LOW);

    Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.
}

String mensaje = "";
String mensaje2 = "";
char incoming_char = 0;
char incoming_char2 = 0;

void loop() {
    llamar(); //Llama
    //mensaje_sms(); //Envia mensaje
    modo_recibe_mensaje();
    for (;;) {

        if (sim900Serial.available() > 0) {

            incoming_char = sim900Serial.read(); //Get the character from the cellular serial port.
            //Serial.print(incoming_char); //Print the incoming character to the terminal.

            if (incoming_char == '\n') {
                
                String mensaje_Ori = "Movil-> " + mensaje;
                const char * c = mensaje_Ori.c_str();
                addText(c);

                mensaje.trim();
                mensaje.toLowerCase();

                if (mensaje == "abrete sesamo" || mensaje == "Abrete sesamo") {
                    digitalWrite(PIN_PUERTA, HIGH);
                    delay(5000);
                    digitalWrite(PIN_PUERTA, LOW);
                }

                mensaje = "";
            }
            else
            {
              mensaje += incoming_char;
            }

        }

        if (Serial.available() > 0) {
            if (Serial.read() == 'A') break;
        }

    }

    addText("OK-2");

    delay(100);
    sim900Serial.println();
    delay(30000);
    while (1); // Espérate por tiempo indefinido

}

int yActual = 17;

void addText(const char * msg) {
    Serial.println(msg);
    int x = 20;

    int sz = 1;

    showmsgXY(x, yActual, sz, NULL, msg);
    yActual = yActual + 17;

    if (yActual > 238) {
        yActual = 17;
        tft.fillScreen(RED);
    }
}

void showmsgXY(int x, int y, int sz,
    const GFXfont * f,
        const char * msg) {
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y + 1, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
    delay(500);
}

void modo_recibe_mensaje() {
    //Configura el modo texto para enviar o recibir mensajes
    sim900Serial.print("AT+CMGF=1\r"); // set SMS mode to text
    delay(100);
    sim900Serial.print("AT+CNMI=2,2,0,0,0\r");

    // blurt out contents of new SMS upon receipt to the GSM shield's serial out
    delay(1000);
}

void llamar()
// Función que permite llamar a un celular local
{
    sim900Serial.println("ATD +34605075405;"); //Celular
    delay(100);
    sim900Serial.println();
    delay(30000); // wait for 30 seconds...
    sim900Serial.println("ATH"); // Cuelta el telefono
    delay(1000);
}

void espera_mensaje() {
    salir = 1;
    while (salir == 1) {
        if (sim900Serial.available() > 0) {
            incoming_char = sim900Serial.read(); //Get the character from the cellular serial port.
            Serial.print(incoming_char); //Print the incoming character to the terminal.
            salir = 0;
        }
    }
}

void mensaje_sms()
//Funcion para mandar mensaje de texto
{

    sim900Serial.print("AT+CMGF=1\r"); // AT command to send SMS message
    delay(100);
    sim900Serial.println("AT+CMGS=\"+34605075405\""); // recipient's mobile number, in international format
    delay(100);
    sim900Serial.println("Saludos desde HetPro"); // message to send
    delay(100);
    sim900Serial.println((char) 26); // End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
    delay(100);
    sim900Serial.println();
    delay(5000); // Tiempo para que se envie el mensaje
    addText("SMS sent successfully");
}
