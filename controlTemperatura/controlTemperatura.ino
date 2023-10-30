/*Sensor de Temperatura contra agua NTC 10K<br>Instrucciones:
El sensor para temperatura contra agua NTC 10k es un thermistor con dos salidas
que requieren de una configuracion pull down. Conecte una terminal del sensor a Vcc 
y la otra utilizando resistencia pull down a tierra y al pin análogo 2.
*/
#include <math.h>
#define pin_releCaldera 6
#define pin_releHumo 5
#define pin_sensorTemperatura A3
#define pin_botonDispararHumo 7

void setup() {
  pinMode(pin_releCaldera, OUTPUT);
  digitalWrite(pin_releCaldera, LOW);

  pinMode(pin_releHumo, OUTPUT);
  digitalWrite(pin_releHumo, LOW);

  pinMode(pin_botonDispararHumo, INPUT);

  Serial.begin(115200);  //Recuerda ajustar to monitor serial a 115200
}

double Thermister(int RawADC) {
  double Temp;
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
  Temp = Temp - 273.15;  // Converierte de Kelvin a Celsius
  //Para convertir Celsius a Farenheith esriba en esta linea: Temp = (Temp * 9.0)/ 5.0 + 32.0;
  return Temp;
}

bool calentando = false;
bool dispararHumo = false;

int tempParaDisparo = 220;
int tempMaxCaldera = 250;
int tempMinCaldera = 220;


void loop() {
    float voltaje, gradosC, gradosF;  
                                     
  voltaje = analogRead(3) * 0.004882814; 
                                            
  gradosC = (voltaje - 0.5) * 60.5;  
               

  bool dispararHumo = gradosC > tempParaDisparo|| gradosC==0;

  if (dispararHumo) {
    Serial.print(" - Disparador Habilitado...  \t ");

      digitalWrite(pin_releHumo, HIGH);

  } else {
    Serial.print(" - Disparador desabilitado.  \t");
    digitalWrite(pin_releHumo, LOW);
  }

  if (calentando && gradosC >= tempMaxCaldera || gradosC==0 ) {

    Serial.println(" - caldera apagando...   C: " + String(gradosC));
    calentando = false;
    digitalWrite(pin_releCaldera, LOW);
  }

  if (calentando == false && gradosC < tempMinCaldera) {
    digitalWrite(pin_releCaldera, HIGH);
    calentando = true;
    Serial.println(" - encediendo caldera...   C: " + String(gradosC));
  } else if (calentando) {
    Serial.println(" - caldera encendida   C: " + String(gradosC));
  } else {
    Serial.println(" - caldera apagada   C: " + String(gradosC));
  }

  delay(1000);  //Espera un segundo y vuelve a escribir
}
//ElectroCrea.com