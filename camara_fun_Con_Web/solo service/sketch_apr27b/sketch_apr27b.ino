#include <WiFi.h>

const char* WIFI_NAME= "MiFibra-1292";  
const char* WIFI_PASSWORD = "yKPbvzF2";  
//const char* WIFI_NAME= "devPablo2_4";  
//const char* WIFI_PASSWORD = "goodlife"; 

// the IP address for the shield:
//IPAddress ip(192, 168, 0, 177);  
WiFiServer server(80);

String header;

String LED_ONE_STATE = "off";
String LED_TWO_STATE = "off";
String LED_THREE_STATE = "off";


const int GPIO_PIN_NUMBER_22 = 14;
const int GPIO_PIN_NUMBER_23 = 14;
const int GPIO_PIN_NUMBER_15 = 4;

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  pinMode(GPIO_PIN_NUMBER_22, OUTPUT);
  //pinMode(GPIO_PIN_NUMBER_23, OUTPUT);
  pinMode(GPIO_PIN_NUMBER_15, OUTPUT);

  digitalWrite(GPIO_PIN_NUMBER_22, LOW);
  //digitalWrite(GPIO_PIN_NUMBER_23, LOW);
  digitalWrite(GPIO_PIN_NUMBER_15, LOW);


  //WiFi.config(ip);

  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  //Serial.print("Connecting to ip: ");
  //Serial.println(ip);
  
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  
  int state = HIGH;
  while (WiFi.status() != WL_CONNECTED) {

      Serial.print("estado:  ");
      Serial.println(WiFi.status());
      delay(10000);
    
      digitalWrite(GPIO_PIN_NUMBER_15, state);
      if (state == HIGH) {
        state = LOW;
        }
      else{
        state = HIGH;
      }

     Serial.print("Trying to connect to Wifi Network");
  }

      delay(500);
    digitalWrite(GPIO_PIN_NUMBER_15, HIGH);
        delay(700);
    digitalWrite(GPIO_PIN_NUMBER_15, LOW);
        delay(300);
    digitalWrite(GPIO_PIN_NUMBER_15, HIGH);
        delay(700);
    digitalWrite(GPIO_PIN_NUMBER_15, LOW);

    
  Serial.println("");
  Serial.println("Successfully connected to WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   
  if (client) {                            
    Serial.println("New Client is requesting web page");          
    String current_data_line = "";               
    while (client.connected() || client.available()) {            
      if (client.available()) {             
        char new_byte = client.read();             
        //Serial.write(new_byte);                    
        header += new_byte;
        if (new_byte == '\n') {                    
         
          if (current_data_line.length() == 0) 
          {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("LED0=ON") != -1) 
            {
              Serial.println("#E");
                digitalWrite(GPIO_PIN_NUMBER_22, HIGH);
                delay(2000);
                digitalWrite(GPIO_PIN_NUMBER_22, LOW);
            } 
            
            if (header.indexOf("LED1=ON") != -1)
            {
                Serial.println("#H");
                digitalWrite(GPIO_PIN_NUMBER_23, HIGH);
                delay(2000);
                digitalWrite(GPIO_PIN_NUMBER_23, LOW);
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");            
            // Web Page Heading
            client.println("</style></head>");
            client.println("<body><center><h1>Control de acceso</h1></center>");
            client.println("<form><center>");
            client.println("<p> Entrada del edificio </p>");
            // If the PIN_NUMBER_22State is off, it displays the ON button      
             client.println("<center> <button class=\"button\" name=\"LED0\" value=\"ON\" type=\"submit\">Abrir entrada</button>") ;
             client.println("<br><br>");
             client.println("<p>Entrada de la casa.. Peligro!!</p>");
             client.println("<button class=\"button\" name=\"LED1\" value=\"ON\" type=\"submit\">Abrir Puerta</button>");
             client.println("<br><br>");
             
            client.println("</center></form></body></html>");
            client.println();
            break;
          } 
          else 
          { 
            current_data_line = "";
          }
        } 
        else if (new_byte != '\r') 
        {  
          current_data_line += new_byte;     
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
