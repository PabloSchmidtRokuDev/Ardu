/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
const char* ssid     = "MOVISTAR_CAE0";
const char* password = "aTchezHwfCpu8MLk6HNn";
#endif

const char* host1 = "192.168.1.140";


const uint16_t port = 80;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  conectarS();
}

  void loop() {

    ShowSerialDataTT();
  }

  char c = ' ';
  String header = "";

  void ShowSerialDataTT()
  {

    if  (Serial.available() && c  != '#') {
      while (Serial.available()) {        // reading data into char array
        c = Serial.read();   // writing data into array
        Serial.write(c);
        header += c;
      }

      if (c == '#') {
        String Linea = "" + header;

        c = ' ';
        Serial.println("Llego: " + Linea);

        if (client.connected()) {
          header = "";
          sendMsjToServerTT(Linea);
        } else {
          
          Serial.println("Servidor no connectado ");
          conectarS();
        }

      }
    }
  }


  void sendMsjToServerTT( String Linea)
  {

    // This will send a string to the server
    //Serial.println("sending data to server");
    if (client.connected()) {
      Serial.print("envio: ");
      Serial.println(Linea);
      client.print(Linea);
    }
  }




  void conectarS()
  {
Serial.println("Conectanndo..... ");
    if (!client.connect(host1, port)) {
      Serial.println("connection failed");
      delay(5000);
      return;
    }
  }



  void desconectarS()
  {

    client.println("");

    // wait for data to be available
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        delay(6000);
        return;
      }
    }

    Serial.println("closing connection-----------------------");
    client.stop();

    // Read all the lines of the reply from server and print them to Serial
    Serial.println("receiving from remote server");
    // not testing 'client.connected()' since we do not need to send data here
    while (client.available()) {
      char ch = static_cast<char>(client.read());
      Serial.print(ch);
    }

    // Close the connection
    Serial.println();
  }
