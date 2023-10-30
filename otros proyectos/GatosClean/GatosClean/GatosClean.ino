int buttonState;
#define timpoTope1 500
#define timpoEsperaIniciarLimpieza 1000
#define timpoEsperaTope2 20
#define distanciaSensor 40

#define pinReleMotor1 3
#define pinReleMotor2 2

#define pinSensorTope1 8
#define pinSensorMedio 7
#define pinSensorTope2 6

#define pinSensorDistancia1 5
#define pinSensorDistancia2 4

#define esperar_tope_1 "esperar_tope_1"
#define esperar_tope_2 "esperar_tope_2"
#define esperar_tope_medio "esperar_tope_medio"

#define regresoInicio "regresoInicio"
#define limpiarPiedras "limpiarPiedras"
#define espera_sensor_distanda "espera_sensor_distanda"
#define espera_sensor_contador "espera_sensor_contador"

void moverMotorDerecha() {
  Serial.println("giro a la derecha");
  digitalWrite(pinReleMotor1, HIGH);
  digitalWrite(pinReleMotor2, HIGH);
}

void moverMotorIzquierda() {
  Serial.println("giro a la izquierda");
  digitalWrite(pinReleMotor1, LOW);
  digitalWrite(pinReleMotor2, HIGH);
}

void pararMotor() {
  Serial.println("paro motore");
  digitalWrite(pinReleMotor1, LOW);
  digitalWrite(pinReleMotor2, LOW);
}

String funcion = "";
String subFuncion = "";
void setup() {

  pinMode(pinReleMotor1, OUTPUT);
  pinMode(pinReleMotor2, OUTPUT);
  pinMode(pinSensorTope1, INPUT);
  pinMode(pinSensorMedio, INPUT);
  pinMode(pinSensorTope2, INPUT);

  pinMode(pinSensorDistancia1, OUTPUT);
  pinMode(pinSensorDistancia2, INPUT);

  Serial.begin(9600);
  Serial.println("serial started at 9600");
  Serial.println("setup completo... start ");
  funcion = regresoInicio;
  subFuncion = "";
  delay(2000);
}




int contandor = -1;
void loop() {
  //Serial.println("********************");
  //Serial.print("funcion: ");
  //Serial.println(funcion);
  //Serial.print("sub funcion: ");
  //Serial.println(subFuncion);
  delay(200);


  //------------reinicio-------------
  if (funcion == regresoInicio) {
    if (subFuncion == "") {
      //Serial.println("regresando al inicio");

      int estadoS = digitalRead(pinSensorTope1);
      //Serial.print("estado sensor tope 1: ");
      //Serial.println(estadoS);

      if (estadoS == HIGH) {
        moverMotorIzquierda();
        Serial.println("espera tope 1");
        subFuncion = esperar_tope_1;
      } else {
        //pararMotor();
        //delay(timpoTope1);
        moverMotorDerecha();
        subFuncion = esperar_tope_medio;
        Serial.println("espera tope medio");
      }

    } else if (subFuncion == esperar_tope_1) {
      if (digitalRead(pinSensorTope1) == LOW) {
        Serial.println("tope 1 alcanzado. espera tiempo x");
        pararMotor();
        delay(timpoTope1);
        moverMotorDerecha();
        subFuncion = esperar_tope_medio;
      }
    }

    else if (subFuncion == esperar_tope_medio) {

      int estadoS = digitalRead(pinSensorMedio);
      //Serial.print("m estado sensor medio: ");
      //Serial.println(estadoS);

      if (estadoS == LOW) {
        Serial.println("tope medio alcanzado. espera tiempo x");

        pararMotor();
        delay(timpoTope1);
        Serial.println("espera sensor distancia");
        funcion = espera_sensor_distanda;
        subFuncion = "";
      }
    }
  }



  //------------limpieza-------------
  if (funcion == limpiarPiedras) {
    if (subFuncion == "") {
      Serial.println("limpiando");

      if (digitalRead(pinSensorTope2) == HIGH) {
        //moverMotorIzquierda();
        moverMotorDerecha();
        Serial.println("espera tope 2");
        subFuncion = esperar_tope_2;
      }
    }

    if (subFuncion == esperar_tope_2) {

      if (digitalRead(pinSensorTope2) == LOW) {
        Serial.println("tope 2 alcanzado. espera tiempo x");

        pararMotor();
        funcion = regresoInicio;
        subFuncion = "";
      }
    }
  }

  //------------espera-------------
  if (funcion == espera_sensor_distanda) {
    int cm = ping(pinSensorDistancia1, pinSensorDistancia2);
    //Serial.print("Distancia: ");
    //Serial.println(cm);
    //delay(500);

    if (subFuncion == "") {
      //Serial.print("distanciaSensor limite: ");
      //Serial.println(distanciaSensor);

      if (cm < distanciaSensor) {
        Serial.print("iniciando cuanta... ");
        contandor = 0;
        subFuncion = espera_sensor_contador;
      }
    }

    else if (subFuncion == espera_sensor_contador) {
      if (contandor >= 0) {
        contandor = contandor + 1;
      }
      
      if (cm < distanciaSensor) {
        Serial.println("reinicia contador");
        contandor = 0;
      } else if (contandor == timpoEsperaIniciarLimpieza) {
        Serial.println("iniciando limpieza: ");
        contandor = -1;
        funcion = limpiarPiedras;
        subFuncion = "";
      }
    }
  }

  /*
        Serial.print("read sernsor 1---->");
        Serial.println(digitalRead(pinSensorTope1));

                Serial.print("read sernsor pinSensorMedio---->");
        Serial.println(digitalRead(pinSensorMedio));

                Serial.print("read pinSensorTope2---->");
        Serial.println(digitalRead(pinSensorTope2));

Serial.println("");
Serial.println("************************");
        delay(1000);


   int cm = ping(pinSensorDistancia1, pinSensorDistancia2);
   if (cm < distanciaSensor){
    contandor = 0;
   }
   Serial.print("Distancia: ");
   Serial.println(cm);
   delay(500);

   if(contandor == timpoEsperaIniciarLimpieza){
      Serial.println("iniciando limpieza: ");
      contandor = -1;
      limpiarPiedras();
   }

   if(contandor >= 0){
     Serial.println("valor contador: ");
      Serial.println(contandor);
      contandor = contandor + 1;
   }
   */
}

int ping(int TriggerPin, int EchoPin) {
  long duration, distanceCm;

  digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos

  distanceCm = duration * 10 / 292 / 2;  //convertimos a distancia, en cm
  return distanceCm;
}