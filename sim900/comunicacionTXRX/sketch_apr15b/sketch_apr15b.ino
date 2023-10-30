
#define RXD2 16
#define TXD2 17
void setup()
{
  
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Iniciamos puerto serial virtual
  Serial.begin(115200);  // Iniciamos puerto serial
  delay(1000);         // Retrasamos 1 segundo
  Serial.println("Iniciado...");
}

void loop()
{
  comunicadatos();
}

void comunicadatos() {
  // Envíamos y recibimos datos
  if (Serial.available() > 0)
    Serial2.write(Serial.read());
  if (Serial2.available() > 0)
    Serial.write(Serial2.read());
}
