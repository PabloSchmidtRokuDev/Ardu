
const int llamadaPorteroPin =  24;

const int puerta1Pin = 22;
const int puerta2Pin = 23;

const String telefonoLlamar =  "ATD +34695687164;";
const String telefonoSms = "";

//SoftwareSerial Serial1(txSim900Pin, rxSim900Pin);         //7 = TX, 8 = RX
unsigned char buffer[64]; //port

void setup(){
  Serial1.begin(9600);     // the Serial1 baud rate   
  Serial.begin(19200);            // the Serial port of Arduino baud rate.

  pinMode(puerta1Pin, OUTPUT);
  pinMode(puerta2Pin, OUTPUT);
  
  pinMode(llamadaPorteroPin, INPUT);

  Serial.print("I'm ready");
  Serial.print("Hello?"); 
}

int count=0;     
int i = 1; //if i = 0, send SMS.
bool llamadaEnProgreso = false;

void loop(){
  if (Serial1.available()){         // if date is comming from softwareserial port ==> data is comming from Serial1 shield

    while(Serial1.available()){          // reading data into char array 
      buffer[count++]=Serial1.read();     // writing data into array
      if(count == 64)break;
    }

    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    String linea = (char*)buffer;
    linea.trim();
    
    Serial.println("....");
    Serial.println(linea);
    Serial.println("....");

    if (linea == "@&e") {
        //addText("Ejecutando accion 1");
        digitalWrite(puerta1Pin, HIGH);
        delay(5000);
        //addText("accion 1 terminado.");
        digitalWrite(puerta1Pin, LOW);
    }
    
    if (linea == "@&h"){
        //addText("ejecutando accion 2");
        digitalWrite(puerta2Pin, HIGH);
        delay(5000);
        //addText("accion 2 terminado.");
        digitalWrite(puerta2Pin, LOW);
    }

    if (linea == "NO CARRIER"){
      llamadaEnProgreso = false;
    }
    
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero
  }

   int llamadaPorteroEstado = digitalRead(llamadaPorteroPin);
    digitalWrite(puerta1Pin, llamadaPorteroEstado);

  if (Serial.available()){  // if data is available on hardwareserial port ==> data is comming from PC or notebook
    
    String serialComand = Serial.readStringUntil('\n');
    serialComand.trim();
    if (serialComand == "LLAMAR"){
      llamar();
    }
    else{
      Serial1.println(serialComand);       // write it to the Serial1 shield
    }

    Serial.print("comando recibido: ");
    Serial.println(serialComand);
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero
    
  }

}

void llamar()
// Función que permite llamar a un celular local
{
  Serial.println(">>>LLAMANDO.....");
  llamadaEnProgreso = true;
  Serial1.println(telefonoLlamar); //Celular
  delay(100);
  Serial1.println();
  Serial.println("Llamando...");
  //delay(30000); // wait for 30 seconds...
  //Serial1.println("ATH"); // Cuelta el telefono
  //delay(1000);
}

void clearBufferArray(){              // function to clear buffer array
  for (int i=0; i<count;i++){
    buffer[i]=NULL;                  // clear all index of array with command NULL
  }
}
