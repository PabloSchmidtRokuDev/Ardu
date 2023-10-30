// uint64_t tDecodedRawData = 0;
// uint32_t tDecodedRawData = 0;
class CodeIR {
public:
  uint64_t decodeHex;
  String strCodigo;

  void setEstado(uint64_t _decodeHex, String _strCodigo);
};

void CodeIR::setEstado(uint64_t _decodeHex, String _strCodigo) {
  decodeHex = _decodeHex;
  strCodigo = _strCodigo;
};

CodeIR listCodeIR[42];

String buscarCodigoEnLista(uint64_t _decodeHex) {

  for (int i = 0; i < 42; i++) {
    if (listCodeIR[i].decodeHex == _decodeHex) return listCodeIR[i].strCodigo;
  }
  return "";
}

void CargarCodeIR() {
  listCodeIR[0].setEstado(0xFF00EF00, "CMD_PowerAll");         //apagar todo
  listCodeIR[1].setEstado(0xFE01EF00, "CMD_PowerCirculos;0");  //apagar circulos
  listCodeIR[2].setEstado(0xFD02EF00, "CMD_PowerCirculos;1");  //encender circulos
  listCodeIR[3].setEstado(0xFC03EF00, "CMD_PowerMegaLed;0");   //apagar megaLed
  listCodeIR[4].setEstado(0xFB04EF00, "CMD_PowerMegaLed;1");   //encender megaled
  listCodeIR[5].setEstado(0xFA05EF00, "CMD_PowerUV;0");        //apagar uv
  listCodeIR[6].setEstado(0xF906EF00, "CMD_PowerUV;1");        //enender uv

  listCodeIR[7].setEstado(0xF807EF00, "CMD_efecto;1");         //efecto 1
  listCodeIR[8].setEstado(0xF708EF00, "CMD_efecto;2");         //efecto 2
  listCodeIR[9].setEstado(0xF609EF00, "CMD_efecto;3");         //efecto 3
  listCodeIR[10].setEstado(0xF50AEF00, "CMD_efecto;4");  //efecto 4
  listCodeIR[11].setEstado(0xF40BEF00, "CMD_efecto;5");  //efecto 5
  listCodeIR[12].setEstado(0xF30CEF00, "CMD_efecto;6");  //efecto 6
  listCodeIR[13].setEstado(0xF20DEF00, "CMD_efecto;7");  //efecto 7

  listCodeIR[14].setEstado(0xF10EEF00, "CMD_Color;1");   //color 1
  listCodeIR[15].setEstado(0xF00FEF00, "CMD_Color;2");   //color 2
  listCodeIR[16].setEstado(0xEF10EF00, "CMD_Color;3");   //color 3
  listCodeIR[17].setEstado(0xEE11EF00, "CMD_Color;4");        //color 4
  listCodeIR[18].setEstado(0xED12EF00, "CMD_Color;5");        //color 5
  listCodeIR[19].setEstado(0xEC13EF00, "CMD_Color;6");        //color 6
  listCodeIR[20].setEstado(0xEB14EF00, "CMD_Color;7");        //color 7
  listCodeIR[21].setEstado(0xEA15EF00, "CMD_Color;8");        //color 8
  listCodeIR[22].setEstado(0xE916EF00, "CMD_Color;9");        //color 9

  listCodeIR[23].setEstado(0xE817EF00, "CMD_brilloCirculoMas");  //brillo 10%
  listCodeIR[24].setEstado(0xBA45FF00, "CMD_brilloCirculoMenos");  //brillo 30%
  listCodeIR[25].setEstado(0xB946FF00, "CMD_brilloMegaLedMas");  //brillo 60%
  listCodeIR[26].setEstado(0xB847FF00, "CMD_brilloMegaLedMenos");                   //brillo 100%
  listCodeIR[27].setEstado(0xBB44FF00, "CMD_powerFinChill");
  listCodeIR[28].setEstado(0xBF40FF00, "");

  listCodeIR[29].setEstado(0xBC43FF00, "CMD_power_Azul;0");
  listCodeIR[30].setEstado(0xF807FF00, "CMD_power_Azul;1");
  listCodeIR[31].setEstado(0xEA15FF00, "CMD_power_Blanco;0");
  listCodeIR[32].setEstado(0xF609FF00, "CMD_power_Blanco;1");

  listCodeIR[33].setEstado(0xE916FF00, "");
  listCodeIR[34].setEstado(0xE619FF00, "");
  listCodeIR[35].setEstado(0xF20DFF00, "");
  listCodeIR[36].setEstado(0xF30CFF00, "");
  listCodeIR[37].setEstado(0xE718FF00, "");
  listCodeIR[38].setEstado(0xA15EFF00, "");
  listCodeIR[39].setEstado(0xE31CFF00, "");
  listCodeIR[40].setEstado(0xA55AFF00, "");
  listCodeIR[41].setEstado(0xAD52FF00, "");
}
