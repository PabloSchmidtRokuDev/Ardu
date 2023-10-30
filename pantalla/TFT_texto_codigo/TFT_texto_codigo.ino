#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
 
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

 int color;

 
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    tft.reset();
    uint16_t identifier = tft.readID();
    Serial.print("ID = 0x");
    Serial.println(identifier, HEX);
    if (identifier == 0xEFEF) identifier = 0x9486;
    tft.begin(identifier);
    
    tft.fillScreen(RED);     // establesemos el color del fondo dela pantalla 

}

//*************************************************************************************************************************************
 
void loop(void)              // inicio de nuestro codigo
{
     tft.setRotation(0);      //establese el display en lapocicion vertical cuatro valoreposible 0,1,2,3,4
    tft.fillRect(10, 10, 220, 40, GREEN); //dibujamos un cuadro y pasamos los parametro eje x, eje y, altura,largo y coloe
    tft.setTextColor(CYAN);   // Definimos el color del texto como CYAN
    tft.setTextSize(3);       //  establecemos el tamañodel texto  
    tft.setCursor(60,130);    // Usamos los valores  comocoordenade del los ej X,Y 
    tft.println("Hola TFT");  // Eescrivimos (Hola TFT) en el display enla cordenada especificada anterio mente

    tft.setTextColor(YELLOW); // Definimos el color del texto como Amarillo
    tft.setTextSize(2);       //  establecemos el tamañodel texto  
    tft.setCursor(60,170);    // Usamos los valores  comocoordenade del los ej X,Y 
    tft.println("Hola TFT");  // Eescrivimos (Hola TFT) en el display enla cordenada especificada anterio mente

    tft.setTextColor(BLUE);   // Definimos el color del texto como azul
    tft.setTextSize(1);       //  establecemos el tamañodel texto  
    tft.setCursor(60,200);    // Usamos los valores  comocoordenade del los ej X,Y 
    tft.println("Hola TFT");  // Eescrivimos (Hola TFT) en el display enla cordenada especificada anterio mente
    tft.fillRect(10, 10, 220, 40, GREEN); //dibujamos uncuadro en laubicacion dode se escrive el nombre del color selecionado
    delay(5000);

    tft.setRotation(1);       //establese el display en lapocicion vertical cuatro valoreposible 0,1,2,3,4
    tft.fillScreen(BLACK);    // establesemos el color del fondo dela negro
    tft.setTextColor(GREEN);  // Definimos el color del texto como verde
    tft.setTextSize(3);       //  establecemos el tamañodel texto  
    tft.setCursor(90,50);     // Usamos los valores  comocoordenade del los ej X,Y 
    tft.println("Hola TFT");  // Eescrivimos (Hola TFT) en el display enla cordenada especificada anterio mente

    tft.fillRect(0, 200, 320, 40, GREEN); ////dibujamos un cuadro y pasamos los parametro eje x, eje y, altura,largo y coloe
    tft.setTextColor(BLUE);  // Definimos el color del texto como negro
    tft.setTextSize(2);       //  establecemos el tamañodel texto  
    tft.setCursor(1,205);    // Usamos los valores  comocoordenade del los ej X,Y 
    tft.println("Electronica y Microcontrol adores");  // escribimos en el centro del cuadroverde
    
    delay(5000);
    tft.fillScreen(RED);     // establesemos el color del fondo dela pantalla 
}

 
