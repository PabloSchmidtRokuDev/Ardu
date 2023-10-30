void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

Serial.println("CMD_PowerAll");
delay(1000);
Serial.println("CMD_powerFinChill");
delay(1000);
Serial.println("CMD_Abrir_Entrada");
delay(1000);
Serial.println("CMD_powerLed");
delay(1000);
Serial.println("CMD_power_flor");
delay(1000);
Serial.println("CMD_efecto");
delay(1000);
Serial.println("CMD_brillo");
delay(1000);
Serial.println("CMD_Plasma");
delay(1000);
Serial.println("CMD_power_UV");
delay(1000);
Serial.println("CMD_audio");
delay(1000);
Serial.println("CMD_Color");
delay(1000);
Serial.println("CMD_Barra");
delay(1000);

}
