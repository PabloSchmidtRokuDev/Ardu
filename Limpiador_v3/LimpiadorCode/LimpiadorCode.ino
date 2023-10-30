const int releEncendidoPin = 3;
const int releDireccionPin = 4;  //low: izquierda  --- high: derecha

const int distEntradaPin = 10;
const int distSalidaPin = 11;

const int sensorTopePin = 9;
const int botonPin = 8;

#define timpoEsperaIniciarLimpieza 200
#define distanciaSensor 20

String estado = "";

void setup() {
  Serial.begin(115200);

  pinMode(releEncendidoPin, OUTPUT);
  pinMode(releDireccionPin, OUTPUT);

  pinMode(distSalidaPin, OUTPUT);  // salida del pulso generado por el sensor ultrasónico
  pinMode(distEntradaPin, INPUT);  // entrada del pulso generado por el sensor ultrasónico

  pinMode(sensorTopePin, INPUT);
  pinMode(botonPin, INPUT);

  Serial.println("ready go");

  volverAlaPosicion0();

  estado = "espara_sensor";
}

int red;
String myString;

int botonBeforeStatus = HIGH;
int contandor = 0;

void loop() {


  int botonStatus = digitalRead(botonPin);

  //Serial.println("estado boton: ");
  //Serial.println(botonStatus);

  if (botonBeforeStatus == LOW && botonStatus == HIGH) {
    botonBeforeStatus = HIGH;
    Serial.println("boton precionado y soltado: ");
    Limpiar();
  } else if (botonBeforeStatus != botonStatus) {
    botonBeforeStatus = botonStatus;
  }

  if (estado == "espara_sensor") {
    // long dis = leerDistancia();

  Serial.print("Distancia: ");
  Serial.println(dis);

    if (dis < distanciaSensor && dis != 0) {
      Serial.println("iniciando cuanta... ");
      contandor = 0;
      estado = "espara_contador";
    }

  } else if (estado == "espara_contador") {

    if (contandor >= 0) {
      contandor = contandor + 1;
            Serial.print("contador: ");
          Serial.println(contandor);
      delay(1000);
    }

 long dis = leerDistancia();

  Serial.print("Distancia: ");
  Serial.println(dis);

    if (dis < distanciaSensor && dis != 0) {
      Serial.println("reinicia contador: ");
      contandor = 0;

    } else if (contandor == timpoEsperaIniciarLimpieza) {
      Serial.println("iniciando limpieza: ");
      contandor = -1;
      Limpiar();
    }
  }
}

void volverAlaPosicion0() {

  Serial.println("ir al punto 0");
//girarTamborDer();
//delay(10000);
  //moverBrazoDireccionHome(5, false);
  int buttonState = digitalRead(sensorTopePin);

  Serial.println("estado boton: ");
  Serial.println(buttonState);

  if (buttonState != LOW) {
    Serial.println("giro  a la izq hasta encontrar el sensor ");
    girarTamborIzq();
  }

  while (buttonState != LOW) {
    delay(1);
    buttonState = digitalRead(sensorTopePin);
  }
  delay(2000);

  Serial.println("cuandoencuentro el sensor giro a la derecha hasta pocicionar las piedras ");
  girarTamborDer();

  delay(10000);

  girarTamborIzq();
  delay(7000);

  girarTamborDer();
    delay(2000);
    
  detenerTambor();

  estado = "espara_contador";

  Serial.println("punto 0 completo");
}

void girarTamborDer() {
  detenerTambor();
  digitalWrite(releEncendidoPin, HIGH);
  digitalWrite(releDireccionPin, HIGH);
}

void girarTamborIzq() {
  detenerTambor();
  digitalWrite(releEncendidoPin, HIGH);
  digitalWrite(releDireccionPin, LOW);
}


void detenerTambor() {
  digitalWrite(releEncendidoPin, LOW);
  digitalWrite(releDireccionPin, LOW);
  delay(2000);
}
void Limpiar() {
  estado = "working";
  Serial.println("Iniciando Limpieza...");

  girarTamborDer();
  delay(4000);

  Serial.println("Giro derecha. Pausa 1/5");
  girarTamborDer();
  delay(2000);

  Serial.println("Giro derecha. Pausa 2/5");
  girarTamborDer();
  delay(3000);


  Serial.println("Giro derecha. Pausa 4/5 ");
  girarTamborDer();
  delay(3000);

  Serial.println("Giro derecha. Pausa 5/5 ");
  girarTamborDer();
  delay(2000);

  Serial.println(" espero un momento...");
  detenerTambor();
  delay(6000);

  volverAlaPosicion0();

  Serial.println("Limpieza completa");
}

long leerDistancia() {
  long dis;
  long tiem;

  digitalWrite(distSalidaPin, LOW);  // recibimiento del pulso.
  delayMicroseconds(4);
  digitalWrite(distSalidaPin, HIGH);  // envió del pulso.
  delayMicroseconds(10);
  digitalWrite(distSalidaPin, LOW);

  tiem = pulseIn(distEntradaPin, HIGH);  // fórmula para medir el pulso entrante.

  dis = tiem * 10 / 292 / 2;  // fórmula para calcular la distancia del sensor ultrasónico.

  return dis;
}