unsigned long lastConnectionTime = 0;        // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 4000;  // delay between updates, in milliseconds


unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
const uint32_t connectTimeoutMs = 2000;


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//char server[] = "https://arduinogetstarted.com/";//http://devpablo.com/Home/Estados.php";
char server[] = "devpablo.com";

// Set the static IP address to use if the DHCP fails to assign
IPAddress et_ip(192, 168, 0, 177);
IPAddress et_myDns(192, 168, 0, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

bool iniciarConexcion() {
  bool conected = false;
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  Ethernet.init(5);  // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1);  // do nothing, no point running without Ethernet hardware
      }
      return false;
    }

    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      return false;
    } else {
      // try to configure using IP address instead of DHCP:
      Ethernet.begin(mac, et_ip, et_myDns);
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      conected = true;
    }

  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
    conected = true;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  lastTime = millis();
  return conected;
}
bool connecting = false;
void httpRequest() {

  if (connecting == false){
  client.flush();
  client.stop();
  //Serial.println("connecting...");
connecting = true;

  if (client.connect(server, 80)) {
    client.println("GET /Home/Estados.php HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    
   // Serial.println("connection finish");
    client.println();
connecting = false;
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    connecting = false;
  }
  lastConnectionTime = millis();
  }
}

void actualizarEstado(String Puerta, String Estado) {
  client.flush();
  client.stop();
  //Serial.println("connecting...");

  if (client.connect(server, 80)) {
    client.println("GET /Home/Estados.php?puerta=" + Puerta + "&estado=" + Estado + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();

  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
  lastConnectionTime = millis();

  lastTime = millis();
}

int starCode = 0;  //0: nada -- 1: numeral encontrado 2: datos validos 3: cierre encontrado
String comando = "";

String VerificarEstadoCliente() {
  if (client.available()) {
    char c = client.read();
    //Serial.write(c);

    if (starCode == 0 && c == '#') {
      starCode = 1;
      comando = "";

    } else if (starCode == 1 && c == '>') {
      starCode = 2;

    } else if (starCode == 2 && c == '<') {
      starCode = 3;

    } else if (c != '<' && starCode == 2) {
      comando = comando + c;

    } else if (starCode == 3 && c == '#') {
      starCode = 0;
      String resultCom = comando;
      resultCom.trim();
      comando = "";
      return resultCom;
    }
  }

  return "";
}