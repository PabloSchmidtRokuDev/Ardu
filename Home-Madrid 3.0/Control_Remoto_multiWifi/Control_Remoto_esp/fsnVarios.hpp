#define CMD_PowerAll "CMD_PowerAll"
#define CMD_powerFinChill "CMD_powerFinChill"
#define CMD_Abrir_Entrada "CMD_Abrir_Entrada"
#define CMD_powerLed "CMD_powerLed"
#define CMD_power_flor "CMD_power_flor"
#define CMD_efecto "CMD_efecto"
#define CMD_brillo "CMD_brillo"
#define CMD_Plasma "CMD_Plasma"
#define CMD_power_UV "CMD_power_UV"
#define CMD_audio "CMD_audio"
#define CMD_Color "CMD_Color"

class deciveAndCommand {
private:
  String deviceName[10] = { "", "", "", "", "", "", "", "", "", "" };
  int deviceNum[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  String command[10] = { "", "", "", "", "", "", "", "", "", "" };
  int conectadosCount = 0;
public:

  void agregarDispocitivo(String _dispocitivoID, int _conexionNum);
  int getConectionCount();
  void setDesconexcion(int _conexionNum);
  void setData(String _command);  // Aqui esta ++
  String getCommand(int _deviceNum);
  int getConextionNum(int _index);
  String getDeviceName(int _index);
};

void deciveAndCommand::agregarDispocitivo(String _dispocitivoID, int _conexionNum) {
  for (int i = 0; i < conectadosCount; i++) {
    if (_dispocitivoID == deviceName[i]) {
      deviceNum[i] = _conexionNum;
      command[i] = "";
      return;
    }
  }

  deviceName[conectadosCount] = _dispocitivoID;
  deviceNum[conectadosCount] = _conexionNum;
  conectadosCount++;
}

int deciveAndCommand::getConectionCount() {
  return conectadosCount;
}

void deciveAndCommand::setDesconexcion(int _conexionNum) {
  for (int i = 0; i < conectadosCount; i++) {
    if (_conexionNum == deviceNum[i]) {
      deviceNum[i] = -1;
      command[i] = "";
    }
  }
}

void deciveAndCommand::setData(String _command) {

  for (int i = 0; i < conectadosCount; i++) {
    command[i] = _command;
    Serial.println("se agrego el commando en la posicion:  " + String(i) + " - " + deviceName[i] + " - " + _command);
  }
}

String deciveAndCommand::getCommand(int _index) {

  String cc = command[_index];
  command[_index] = "";
  Serial.println("comando encontrado en:  " + String(_index) + " - " + deviceName[_index] + " - " + cc);

  return cc;
}

int deciveAndCommand::getConextionNum(int _index) {

  return deviceNum[_index];
}

String deciveAndCommand::getDeviceName(int _index) {

  return deviceName[_index];
}

deciveAndCommand listDeciveAndCommand;

void executeFunction(String CMD) {
  Serial.print("executeFunction.. : ");

  if (CMD == CMD_Abrir_Entrada) {
    Serial.println("Open door Entrada");
    digitalWrite(PIN_Entrada, HIGH);
    delay(1000);
    digitalWrite(PIN_Entrada, LOW);

  } else if (CMD == CMD_efecto) {
    listDeciveAndCommand.setData(CMD_efecto);
  } else {
    /*
    CMD_brillo
    CMD_Color
    CMD_PowerAll
    CMD_powerFinChill
    CMD_powerLed
    CMD_power_flor
    CMD_powerFinChill
    CMD_Plasma
    CMD_power_UV
    */
    Serial.println("setData.. ");
    listDeciveAndCommand.setData(CMD);
  }
}

bool ShowSerialData() {
  int bufferCount;
  if (Serial2.available()) {

    String serialHeader = "";

    delay(50);
    bufferCount = 0;
    while (Serial2.available()) {  // reading data into char array
      char c = Serial2.read();     // writing data into array
      Serial.print(c);
      if (c == '\n') break;

      if (bufferCount == 100) break;
      bufferCount++;

      serialHeader += c;
    }

    Serial.println("llegsa: " + serialHeader);
    serialHeader.trim();

    if (serialHeader.length() >= 3) {

      executeFunction(serialHeader);
      Serial.println("se agrega comando a la lista " + serialHeader);

      return true;
    }
  }

  return false;
}
