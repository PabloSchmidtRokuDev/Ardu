
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;


#include <Servo.h>

const char* ssid = "MOVISTAR_CAE0";
const char* password = "aTchezHwfCpu8MLk6HNn";

#define PIN_Entrada 2
#define PIN_Medio 0
#define PIN_PuertaA 4
#define PIN_PuertaB 5

#define PIN_BotonAbrirPuertaA 15
#define PIN_BotonAbrirMedio 13
#define PIN_ECO 12

String serverEstados = "http://devpablo.com/Home/Estados.php";

const uint16_t port = 80;

// Variable to store the HTTP request
String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 122);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);    // optional
IPAddress secondaryDNS(8, 8, 4, 4);  // optional

Servo servo1;

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
const uint32_t connectTimeoutMs = 2000;

//#include <Buzzer.h>
//Buzzer buzzer(PIN_BEEP);

void executeBeep(int beepN) {
  //   if(beepN == 0){
  //       buzzer.begin(100);
  // buzzer.sound(NOTE_G6, 80);
  //   buzzer.sound(NOTE_C5, NOTE_C3);

  //   buzzer.end(1000);
  //   } else if(beepN == 1)  {
  //       buzzer.begin(500);

  //   buzzer.sound(NOTE_C5, NOTE_C3);
  //  buzzer.sound(NOTE_C5, NOTE_C3);
  //   buzzer.end(1000);
  //   }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start....");


  //pinMode(PIN_BotonAbrirPuertaA, INPUT);
  pinMode(PIN_Entrada, OUTPUT);
  pinMode(PIN_Medio, OUTPUT);
  pinMode(PIN_PuertaA, OUTPUT);
  pinMode(PIN_PuertaB, OUTPUT);


  servo1.attach(PIN_Medio);
  servo1.write(0);
  servo1.detach();

  digitalWrite(PIN_PuertaA, LOW);
  digitalWrite(PIN_PuertaB, LOW);
  digitalWrite(PIN_Entrada, LOW);
  

  wifiMulti.addAP("movil4G", "goodlife");

  wifiMulti.addAP("PabloDev", "goodlife");
  wifiMulti.addAP("PabloDev_2_4", "goodlife");
  wifiMulti.addAP("MOVISTAR_CAE0", "aTchezHwfCpu8MLk6HNn");

  //conectarInternet();


  // Configures static IP address
 // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
 //   Serial.println("STA Failed to configure");
 // }
 WiFi.disconnect(false);
 Serial.println("Start1....");
  WiFi.begin(ssid, password);
 Serial.println("Start.2...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 // Print local IP address and start web server
    Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  executeBeep(1);

  lastTime = millis();
}

int botonAbrirPuertaA = LOW;
int botonTimbre = LOW;
        WiFiClient client;
        HTTPClient http;

void loop() {
  //if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
  //if (WiFi.status() == WL_CONNECTED) {
    // Serial.print("WiFi connected: ");
    // Serial.print(WiFi.SSID());
    // Serial.print(" ");
    // Serial.println(WiFi.localIP());

    if ((millis() - lastTime) > timerDelay) {
      //Serial.print("[HTTP] begin...\n");

      // Serial.print(serverEstados);
      // Serial.print(':');
      // Serial.println(port);

      //Check WiFi connection status
      if (WiFi.status() == WL_CONNECTED) {

        String serverPath = serverEstados;

        // Your Domain name with URL path or IP address with path
        http.begin(client, serverPath.c_str());

        // Send HTTP GET request
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println("from hosting estados: " + payload);

          // 0123456
          // 0,0,0,0

          Serial.println(payload.substring(0, 1));
          Serial.println(payload.substring(2, 3));
          Serial.println(payload.substring(4, 5));
          Serial.println(payload.substring(6, 7));

          if (payload.length() == 7) {
            if (payload.substring(0, 1) == "1")
              openDoorNum(0);

            if (payload.substring(2, 3) == "1")
              openDoorNum(1);

            if (payload.substring(4, 5) == "1")
              openDoorNum(2);

            if (payload.substring(6, 7) == "1")
              openDoorNum(3);
          }

          Serial.println(payload);
        } else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);

        }
        // Free resources
        http.end();
       
      } else {
        Serial.println("WiFi Disconnected ");
        // ConsultarEstado();
        // ShowSerialData();
      }
      lastTime = millis();
    }
  //} else {
  //  Serial.println("WiFi not connected!");
  //  Serial.println("WiFi Disconnected - Consultado estado por 4g");
    // ConsultarEstado();
    // ShowSerialData();
  //}

  // botonAbrirPuertaA = digitalRead(PIN_BotonAbrirPuertaA);
  // if (botonAbrirPuertaA == HIGH)
  // {
  //   Serial.println("Abriendo por el Boton...");
  //   digitalWrite(PIN_PuertaA, HIGH);
  //   delay(7000);
  //   digitalWrite(PIN_PuertaA, LOW);
  // }
}

void conectarInternet() {
  /* Configure bearer profile 1 */
  //addText("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  Serial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""); /* Connection type GPRS */
  delay(500);
  ShowSerialData();

  //addText("AT+SAPBR=3,1,\"APN\",\"TATA.DOCOMO.INTERNET\"");
  Serial.println("AT+SAPBR=3,1,\"APN\",\"TATA.DOCOMO.INTERNET\""); /* APN of the provider */
  delay(500);
  ShowSerialData();

  //addText("AT+SAPBR=1,1");
  Serial.println("AT+SAPBR=1,1"); /* Open GPRS context */
  delay(500);
  ShowSerialData();

  //addText("AT+SAPBR=2,1");
  Serial.println("AT+SAPBR=2,1"); /* Query the GPRS context */
  delay(500);
  ShowSerialData();

  //addText("AT+HTTPINIT");
  Serial.println("AT+HTTPINIT"); /* Initialize HTTP service */
  delay(500);
  ShowSerialData();

  //Serial.print("AT+HTTPPARA=\"CID\",1");
  Serial.println("AT+HTTPPARA=\"CID\",1"); /* Set parameters for HTTP session */
  delay(500);
  ShowSerialData();
}

void ConsultarEstado() {
  String valor = "";  //"AT+HTTPPARA=\"URL\",\"http://devpablo.com/saveLL.php?Lat=" + Lat + "&Lon=" + Lon;
  //addText("Enviando coordenadas...");
  //addText(valor);
  Serial.println("AT+HTTPPARA=\"URL\",\"" + serverEstados + "\""); /* Set parameters for HTTP session */
  delay(500);
  ShowSerialData();

  //addText("AT+HTTPACTION=0 - Start GET session");
  Serial.println("AT+HTTPACTION=0"); /* Start GET session */
  delay(500);
  ShowSerialData();

  //addText("AT+HTTPREAD - Read data from HTTP server");
  Serial.println("AT+HTTPREAD"); /* Read data from HTTP server */
  delay(2000);
  ShowSerialData();
  //addText("Envio completo...");
}

bool ShowSerialData() {
  int count = 0;
  unsigned char buffer[64];  //port

  for (int i = 0; i < count; i++) {
    buffer[i] = NULL;  // clear all index of array with command NULL
  }

  while (Serial.available()) {        // reading data into char array
    buffer[count++] = Serial.read();  // writing data into array
    if (count == 64) break;
  }

  //Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
  String linea = (char*)buffer;
  linea.trim();

  //Serial.println("sim900: " + linea);
  // addText("sim900: " + linea);
  Serial.println(">> " + linea);
  if (linea == "OK") return true;
  else return false;
}

void openDoorNum(int doorNum) {
  if (doorNum == 0) {
    Serial.println("Opeb door Entrada");
    digitalWrite(PIN_Entrada, HIGH);
    delay(1000);
    digitalWrite(PIN_Entrada, LOW);
  }

  if (doorNum == 1) {
    Serial.println("Opeb door medio");
    servo1.attach(PIN_Medio);
    delay(100);
    servo1.write(180);
    delay(5000);
    servo1.write(0);
    delay(100);
    servo1.detach();
  }

  if (doorNum == 2) {
    Serial.println("Opeb door puerta A");
    digitalWrite(PIN_PuertaA, HIGH);
    delay(3000);
    digitalWrite(PIN_PuertaA, LOW);
  }

  if (doorNum == 3) {
    Serial.println("Opeb door B");
    digitalWrite(PIN_PuertaB, HIGH);
    delay(1000);
    digitalWrite(PIN_PuertaB, LOW);
  }
}
