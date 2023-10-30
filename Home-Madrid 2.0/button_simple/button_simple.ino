#if 1

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=143,TS_RT=861,TS_TOP=117,TS_BOT=920;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button btn_power, btn_uv, btn_bola, btn_led, btn_redMas, btn_redMenos, btn_greenMas, btn_greenMenos, btn_blueMas, btn_blueMenos, btn_efectoMas, btn_EfectoMenos;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);

    btn_power.initButton(&tft,  60, 40, 100, 40, WHITE, CYAN, BLACK, "Power", 2);
    btn_uv.initButton   (&tft, 180, 40, 100, 40, WHITE, CYAN, BLACK, "Uv", 2);
    
    btn_bola.initButton(&tft,  60, 40 * 2 + 10, 100, 40, WHITE, CYAN, BLACK, "Bola", 2);
    btn_led.initButton (&tft, 180, 40 * 2 + 10, 100, 40, WHITE, CYAN, BLACK, "Led", 2);

    btn_redMas.initButton  (&tft,  60, 40 * 3 + 10 * 3, 100, 40, WHITE, CYAN, BLACK, "Red +", 2);
    btn_redMenos.initButton(&tft, 180, 40 * 3 + 10 * 3, 100, 40, WHITE, CYAN, BLACK, "Red  -", 2);

    btn_greenMas.initButton  (&tft,  60, 40 * 4 + 10 * 4, 100, 40, WHITE, CYAN, BLACK, "gree +", 2);
    btn_greenMenos.initButton(&tft, 180, 40 * 4 + 10 * 4, 100, 40, WHITE, CYAN, BLACK, "green -", 2);

    btn_blueMas.initButton   (&tft,  60, 40 * 5 + 10 * 5, 100, 40, WHITE, CYAN, BLACK, "blue +", 2);
    btn_blueMenos.initButton (&tft, 180, 40 * 5 + 10 * 5, 100, 40, WHITE, CYAN, BLACK, "blue -", 2);

    btn_efectoMas.initButton (&tft,  60, 40 * 6 + 10 * 6, 100, 40, WHITE, CYAN, BLACK, "efecto +", 2);
    btn_EfectoMenos.initButton(&tft, 180, 40 * 6 + 10 * 6, 100, 40, WHITE, CYAN, BLACK, "efecto -", 2);

    
    btn_power.drawButton(false);
    btn_uv.drawButton(false);
    btn_bola.drawButton(false);
    btn_led.drawButton(false);
    btn_redMas.drawButton(false);
    btn_redMenos.drawButton(false);
    btn_greenMas.drawButton(false);
    btn_greenMenos.drawButton(false);
    btn_blueMas.drawButton(false);
    btn_blueMenos.drawButton(false);
    btn_efectoMas.drawButton(false);
    btn_EfectoMenos.drawButton(false);

   // tft.fillRect(40, 80, 160, 80, RED);
}

/* two buttons are quite simple
 */
void loop(void)
{
    bool down = Touch_getXY();

    btn_power.press(down && btn_power.contains(pixel_x, pixel_y));
    btn_uv.press(down && btn_uv.contains(pixel_x, pixel_y));
    btn_bola.press(down && btn_bola.contains(pixel_x, pixel_y));
    btn_led.press(down && btn_led.contains(pixel_x, pixel_y));
    btn_redMas.press(down && btn_redMas.contains(pixel_x, pixel_y));
    btn_redMenos.press(down && btn_redMenos.contains(pixel_x, pixel_y));
    btn_greenMas.press(down && btn_greenMas.contains(pixel_x, pixel_y));
    btn_greenMenos.press(down && btn_greenMenos.contains(pixel_x, pixel_y));
    btn_blueMas.press(down && btn_blueMas.contains(pixel_x, pixel_y));
    btn_blueMenos.press(down && btn_blueMenos.contains(pixel_x, pixel_y));
    btn_efectoMas.press(down && btn_efectoMas.contains(pixel_x, pixel_y));
    btn_EfectoMenos.press(down && btn_EfectoMenos.contains(pixel_x, pixel_y));


    if (btn_power.justReleased()) btn_power.drawButton(true);
    if (btn_uv.justReleased()) btn_uv.drawButton(true);
    if (btn_bola.justReleased()) btn_bola.drawButton(true);
    if (btn_led.justReleased()) btn_led.drawButton(true);
    if (btn_redMas.justReleased()) btn_redMas.drawButton(true);
    if (btn_redMenos.justReleased()) btn_redMenos.drawButton(true);
    if (btn_greenMas.justReleased()) btn_greenMas.drawButton(true);
    if (btn_greenMenos.justReleased()) btn_greenMenos.drawButton(true);
    if (btn_blueMas.justReleased()) btn_blueMas.drawButton(true);
    if (btn_blueMenos.justReleased()) btn_blueMenos.drawButton(true);
    if (btn_efectoMas.justReleased()) btn_efectoMas.drawButton(true);
    if (btn_EfectoMenos.justReleased()) btn_EfectoMenos.drawButton(true);

    //if (on_btn.justPressed()) {  on_btn.drawButton(true);  tft.fillRect(40, 80, 160, 80, GREEN); }
    //if (off_btn.justPressed()) { off_btn.drawButton(true);  tft.fillRect(40, 80, 160, 80, RED); }
    //if (on_btn.justPressed()) {  on_btn.drawButton(true);  tft.fillRect(40, 80, 160, 80, GREEN); }
    if (btn_power.justPressed()){Serial.println("btn pressed"); btn_power.drawButton(true);}
    if (btn_uv.justPressed()){Serial.println("btn pressed"); btn_uv.drawButton(true);}
    if (btn_bola.justPressed()){Serial.println("btn pressed"); btn_bola.drawButton(true);}
    if (btn_led.justPressed()){Serial.println("btn pressed"); btn_led.drawButton(true);}
    if (btn_redMas.justPressed()){Serial.println("btn pressed"); btn_redMas.drawButton(true);}
    if (btn_redMenos.justPressed()){Serial.println("btn pressed"); btn_redMenos.drawButton(true);}
    if (btn_greenMas.justPressed()){Serial.println("btn pressed"); btn_greenMas.drawButton(true);}
    if (btn_greenMenos.justPressed()){Serial.println("btn pressed"); btn_greenMenos.drawButton(true);}
    if (btn_blueMas.justPressed()){Serial.println("btn pressed"); btn_blueMas.drawButton(true);}
    if (btn_blueMenos.justPressed()){Serial.println("btn pressed"); btn_blueMenos.drawButton(true);}
    if (btn_efectoMas.justPressed()){Serial.println("btn pressed"); btn_efectoMas.drawButton(true);}
    if (btn_EfectoMenos.justPressed()){Serial.println("btn pressed"); btn_EfectoMenos.drawButton(true);}
}

void reDrawBtn(){
     btn_power.drawButton();
     btn_uv.drawButton();
     btn_bola.drawButton();
     btn_led.drawButton();
     btn_redMas.drawButton();
     btn_redMenos.drawButton();
     btn_greenMas.drawButton();
     btn_greenMenos.drawButton();
     btn_blueMas.drawButton();
     btn_blueMenos.drawButton();
     btn_efectoMas.drawButton();
     btn_EfectoMenos.drawButton();
}
#endif
