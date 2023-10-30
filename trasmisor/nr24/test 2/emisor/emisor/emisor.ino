#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10

 #define trigPin 5
#define echoPin 6

//Variable con la dirección del canal por donde se va a transmitir
byte direccion[5] ={'c','a','n','a','l'};

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector con los datos a enviar
float datos[3];

void setup()
{

    pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //inicializamos el NRF24L01 
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600); 
 
//Abrimos un canal de escritura
 radio.openWritingPipe(direccion);
 
}

int Limite = 140 ;
 
void loop()
{ 

    long duracion, distancia ;

  digitalWrite(trigPin, LOW);        // Nos aseguramos de que el trigger está desactivado
  delayMicroseconds(2);              // Para asegurarnos de que el trigger esta LOW
  digitalWrite(trigPin, HIGH);       // Activamos el pulso de salida
  delayMicroseconds(10);             // Esperamos 10µs. El pulso sigue active este tiempo
  digitalWrite(trigPin, LOW);        // Cortamos el pulso y a esperar el echo

  duracion = pulseIn(echoPin, HIGH) ;
  distancia = duracion / 2 / 29.1  ;
  if (distancia > Limite) distancia = Limite;
  
 //cargamos los datos en la variable datos[]
 datos[0]=distancia;
 datos[1]=millis();
 datos[2]=3.14;
 //enviamos los datos
 bool ok = radio.write(datos, sizeof(datos));
  //reportamos por el puerto serial los datos enviados 
  if(ok)
  {
     Serial.print("Datos enviados: "); 
     Serial.print(datos[0]); 
     Serial.print(" , "); 
     Serial.print(datos[1]); 
     Serial.print(" , "); 
     Serial.println(datos[2]); 
  }
  else
  {
     Serial.println("no se ha podido enviar");
  }
  delay(1000);
}
