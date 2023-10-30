long dis;
long tiem;

int pinEntrada = 12;
int pinSalida =13;

void setup( ){
  Serial.begin(9600);
  pinMode(pinSalida, OUTPUT); //salida del pulso generado por el sensor ultrasónico
  pinMode(pinEntrada, INPUT);//entrada del pulso generado por el sensor ultrasónico
}

void loop(){
  digitalWrite(pinSalida,LOW);//recibimiento del pulso.
  delayMicroseconds(5);
  digitalWrite(pinSalida, HIGH);//envió del pulso.
  delayMicroseconds(10);
  
  tiem=pulseIn(pinEntrada, HIGH);//fórmula para medir el pulso entrante.
  
  dis= long(0.017*tiem);//fórmula para calcular la distancia del sensor ultrasónico.
  
  //if(dis>10){ //comparativo para la alarma se ingresa la distancia en la que encenderá o apagara.
  //  digitalWrite(11, HIGH);
  //}
  //else
  //{
  //  digitalWrite(11,LOW);
  //}
  
  Serial.println("LA DISTANCIA MEDIDA ES:");
  Serial.println(dis);
  Serial.println("cm");
  delay(500);
}
