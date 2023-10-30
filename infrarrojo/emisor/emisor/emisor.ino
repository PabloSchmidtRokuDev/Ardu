#include <Arduino.h>
#include "PinDefinitionsAndMore.h"//Configuración y definición de pines
#include <IRremote.hpp>
#include <WiFi.h>
const char* ssid     = "PabloDev_2_4";
const char* password = "goodlife";
//configuración ip estática para esp32
IPAddress ip(192,168,1,78);     
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0);
WiFiServer server(80);//puesto 80
void setup() {
    Serial.begin(115200);
    IrSender.begin(); // Inicializamos el emisor infrarrojo
    Serial.print(F("Listo para enviar señales IR en el pin"));
    Serial.println(IR_SEND_PIN);//Muestra el número del pin configurado en PinDefinitionsAndMore.h
     // Comenzamos conectándonos a una red WiFi
    Serial.println();
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);
    // WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);//Configuración ip estática
    WiFi.begin(ssid, password);//Inicilizamos con los datos de nuestra red wifi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Conectado a red Wifi.");
    Serial.println("Dirección IP ");
    Serial.println(WiFi.localIP());   
    server.begin();//Inicializamos el servidor web en el puerto 80
}
void loop() {
WiFiClient client = server.available();   // Escuchando a los clientes entrantes

  if (client) {                             // Si hay un cliente,
    Serial.println("Nuevo cliente");        // Imprime un mensaje en el puerto serie
    String currentLine = "";                // String para contener datos entrantes del cliente
    while (client.connected()) {            // Bucle mientras el cliente está conectado
      if (client.available()) {             // Si hay bytes para leer del cliente,
        char c = client.read();             // Lee un caracter
        Serial.write(c);                    // Lo imprimimos en el monitor serial
        if (c == '\n') {                    // Si el byte es un carácter de nueva línea

          
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Contenido HTML
            client.print("Click <a href=\"/POWER\">POWER</a> Tecla POWER.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/VOL+\">VOLT+</a> Tecla subir volumen.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/VOLT-\">VOL-</a> Tecla bajar el volumen.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/MENU\">MENU</a> Tecla MENU.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/EXIT\">EXIT</a> Tecla Salir.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/INFO\">INFO</a> Tecla INFO.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/TOOLS\">TOOLS</a> Tecla TOOLS.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/SOURCE\">SOURCE</a> Tecla elegir fuente de video.<br>");         
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/ARRIBA\">ARRIBA</a> Tecla ARRIBA.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/DERECHA\">DERECHA</a> Tecla DERECHA.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/ABAJO\">ABAJO</a> Tecla ABAJO.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/IZQUIERDA\">IZQUIERDA</a> Tecla IZQUIERDA.<br>");
//-------------------------------------------------------------------------------------------
            client.print("Click <a href=\"/OK\">OK</a> Tecla OK confirmar.<br>");
//-------------------------------------------------------------------------------------------                   
            client.println();
            // Salir del ciclo while:
            break;
          } else {    // si tienes una nueva línea, borra currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
//----------------------------------
        if (currentLine.endsWith("GET /POWER")) {
             IrSender.sendSAMSUNG(0xE0E040BF, 32); //apagar encender
        }                      
        //-----------------------
        if (currentLine.endsWith("GET /VOL+")) {       
         IrSender.sendSAMSUNG(0xE0E0E01F, 32);   //Volumen +      
        }
        //-----------------------
        if (currentLine.endsWith("GET /VOL-")) { 
         IrSender.sendSAMSUNG(0xE0E0D02F, 32); //volumen -          
        }
        //-----------------------
        if (currentLine.endsWith("GET /MENU")) {
           IrSender.sendSAMSUNG(0xE0E058A7, 32); //menu              
        }
        //-----------------------
        if (currentLine.endsWith("GET /EXIT")) {
          IrSender.sendSAMSUNG(0xE0E0B44B, 32);   //Exit/salir        
        }
        //-----------------------
        if (currentLine.endsWith("GET /INFO")) {
          IrSender.sendSAMSUNG(0xE0E0F807, 32);//Info
        }
        //-----------------------              
        if (currentLine.endsWith("GET /TOOLS")) {
          IrSender.sendSAMSUNG(0xE0E0D22D, 32);// Tools             
        }
        //-----------------------
        if (currentLine.endsWith("GET /SOURCE")) {
          IrSender.sendSAMSUNG(0xE0E0807F, 32);//Source 
          }   
        //-----------------------
        if (currentLine.endsWith("GET /ARRIBA")) {
          IrSender.sendSAMSUNG(0xE0E006F9, 32);//ARRIBA          
        }      
        //-----------------------
        if (currentLine.endsWith("GET /DERECHA")) {
          IrSender.sendSAMSUNG(0xE0E046B9, 32);//DERECHA        
        }
        //-----------------------
        if (currentLine.endsWith("GET /ABAJO")) {
          IrSender.sendSAMSUNG(0xE0E08679, 32);//ABAJO          
        }
        //-----------------------
        if (currentLine.endsWith("GET /IZQUIERDA")) {
          IrSender.sendSAMSUNG(0xE0E0A659, 32);//IZQUIERDA        
        }
        //-----------------------
        if (currentLine.endsWith("GET /OK")) {
          IrSender.sendSAMSUNG(0xE0E016E9, 32);//Source          
        }      
      }
    }
    // Cierra la conexión
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
