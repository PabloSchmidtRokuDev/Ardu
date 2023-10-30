//#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <Wire.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;  
int pox;   
int poy;
//int poy2;
int poy23;
int servoPin1 = 12;
int servoPin2 = 13;
int servoPin3 = 14;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  int x;
  int y;
  int y2;
}struct_message;

typedef struct struct_message2 {
  int id;
  int y2;
}struct_message2;
// Create a struct_message called myData
struct_message myData;
struct_message2 myData2;
// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[3] = {board1, board2, board3};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
    Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
    boardsStruct[myData.id-1].x = myData.x;
    boardsStruct[myData.id-1].y = myData.y;
    Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
    Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
    Serial.println();
    boardsStruct[myData2.id-2].y2 = myData2.y2;
    Serial.printf("y2 value: %d \n", boardsStruct[myData2.id-2].y2);
    Serial.println();
}

void setup() {
// attaches the servo 
  myservo1.attach(servoPin1, 500, 2400); 
  myservo2.attach(servoPin2, 500, 2400); 
  myservo3.attach(servoPin3, 500, 2400); 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

 
}
 
void loop() { 
  int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y; 
  //int board2Y = boardsStruct[1].y2;
  int poy22 = boardsStruct[myData2.id-2].y2;
  pox = board1X;
  poy = board1Y;
  //poy2 = board2Y; 
  poy23 = poy22;
  Posx();
  Posy();
  Poy2();
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  int board3X = boardsStruct[2].x;
  int board3Y = boardsStruct[2].y;*/

  delay(10);  
}

void Posx(){
     if (pox>=0 && pox<=45)
          pox = (pox*2)+180;
     else
         pox = (pox-360)*2;
         myservo1.write(pox);
         //Serial.println(pox);  
}

void Posy(){
     if (poy>=0 && poy<=45)
          poy = (poy*2)+180;
     else
         poy = (poy-360)*2;
         myservo2.write(poy);
        // Serial.println(poy);  
}
void Poy2(){
     if (poy23>=0 && poy23<=45)
          poy23 = (poy23*2)+180;
     else
         poy23 = (poy23-360)*2;
         myservo3.write(poy23); 
}