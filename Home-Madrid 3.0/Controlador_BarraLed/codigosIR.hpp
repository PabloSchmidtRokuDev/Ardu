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

CodeIR listCodeIR[21];

String buscarCodigoEnLista(uint64_t _decodeHex) {

  for (int i = 0; i < 21; i++) {
    if (listCodeIR[i].decodeHex == _decodeHex) return listCodeIR[i].strCodigo;
  }
  return "";
}

void CargarCodeIR() {
  listCodeIR[0].setEstado(0xBA45FF00, "CMD_PowerAll");         //apagar todo/encender
  listCodeIR[1].setEstado(0xB946FF00, "CMD_Mode");  //modo: Color/efecto
  listCodeIR[2].setEstado(0xB847FF00, "CMD_");  //mute: nada
  listCodeIR[3].setEstado(0xBB44FF00, "CMD_");   //play: nada
  listCodeIR[4].setEstado(0xBF40FF00, "CMD_");   //atrazar: nada
  listCodeIR[5].setEstado(0xBC43FF00, "CMD_");        //adelantar: nada
  listCodeIR[6].setEstado(0xF807FF00, "CMD_");        //eq: nada

  listCodeIR[7].setEstado(0xEA15FF00, "CMD_BrilloMenos");         //valomen menos: bajarBrillo
  listCodeIR[8].setEstado(0xF609FF00, "CMD_BrilloMas");         //volumen mas: subir brillo
  listCodeIR[9].setEstado(0xE916FF00, "CMD_Blanco");         //0: blanco


  listCodeIR[10].setEstado(0xE916FF00, "CMD_");  //rpt: nada
  listCodeIR[11].setEstado(0xE916FF00, "CMD_");  //u/sd: nada
  listCodeIR[12].setEstado(0xE619FF00, "CMD_efectoColor;1");  //1: efecto1 /color 1
  listCodeIR[13].setEstado(0xF20DFF00, "CMD_efectoColor;2");  //2: efecto2 /color 2
  listCodeIR[14].setEstado(0xF30CFF00, "CMD_efectoColor;3");   //3: efecto3 /color 3
  listCodeIR[15].setEstado(0xE718FF00, "CMD_efectoColor;4");   //4: efecto4 /color 4
  listCodeIR[16].setEstado(0xA15EFF00, "CMD_efectoColor;5");   //5: efecto5 /color 5
  listCodeIR[17].setEstado(0xE31CFF00, "CMD_efectoColor;6");        //6: efecto6 /color 6
  listCodeIR[18].setEstado(0xA55AFF00, "CMD_efectoColor;7");        //7: efecto7 /color 7
  listCodeIR[19].setEstado(0xAD52FF00, "CMD_efectoColor;8");        //8: efecto8 /color 8
  listCodeIR[20].setEstado(0xB54AFF00, "CMD_efectoColor;9");        //9: efecto9 /color 9







  // listCodeIR[21].setEstado(0xEA15EF00, "CMD_Color;8");        //color 8
  // listCodeIR[22].setEstado(0xE916EF00, "CMD_Color;9");        //color 9

  // listCodeIR[23].setEstado(0xE817EF00, "CMD_brilloCirculoMas");  //brillo 10%
  // listCodeIR[24].setEstado(0xBA45FF00, "CMD_brilloCirculoMenos");  //brillo 30%
  // listCodeIR[25].setEstado(0xB946FF00, "CMD_brilloMegaLedMas");  //brillo 60%
  // listCodeIR[26].setEstado(0xB847FF00, "CMD_brilloMegaLedMenos");                   //brillo 100%
  // listCodeIR[27].setEstado(0xBB44FF00, "CMD_powerFinChill");
  // listCodeIR[28].setEstado(0xBF40FF00, "");

  // listCodeIR[29].setEstado(0xBC43FF00, "CMD_power_Azul;0");
  // listCodeIR[30].setEstado(0xF807FF00, "CMD_power_Azul;1");
  // listCodeIR[31].setEstado(0xEA15FF00, "CMD_power_Blanco;0");
  // listCodeIR[32].setEstado(0xF609FF00, "CMD_power_Blanco;1");

  // listCodeIR[33].setEstado(0xE916FF00, "");
  // listCodeIR[34].setEstado(0xE619FF00, "");
  // listCodeIR[35].setEstado(0xF20DFF00, "");
  // listCodeIR[36].setEstado(0xF30CFF00, "");
  // listCodeIR[37].setEstado(0xE718FF00, "");
  // listCodeIR[38].setEstado(0xA15EFF00, "");
  // listCodeIR[39].setEstado(0xE31CFF00, "");
  // listCodeIR[40].setEstado(0xA55AFF00, "");
  // listCodeIR[41].setEstado(0xAD52FF00, "");
}
