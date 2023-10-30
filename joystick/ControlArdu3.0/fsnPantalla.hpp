void selectOptionMenu(int selOpt) {
  if (selOpt == 1) {  // luz 01
    opcionSelect = 1;
    crearPantallaLuz01();
    updatePantallaLuz01("cnn_exito", "");

  } else if (selOpt ==2) {  // soporte
    opcionSelect = 2;
    crearPantallaSoporte();
    updatePantallaSoporte("cnn_exito", "");

  } else if (selOpt == 3) {  // brazo 4 puntos
    opcionSelect = 3;
    crearPantallaBrazo01("Brazo Rojo 01");
    updatePantallaBrazo01("cnn_exito", "");

  } else if (selOpt == 4) {  // brazo 4 puntos
    opcionSelect = 4;
    crearPantallaBrazo01("Brazo Rojo 02");
    updatePantallaBrazo01("cnn_exito", "");
  }
}

void executeInstruccion(String obj, String cm) {

  if (opcionSelect == 0) {  //menu principal
    if (obj == "XA" && cm == "0" && subOpcionSelect > 0) {
      updateMenu(subOpcionSelect - 1);
    } else if (obj == "XB" && cm == "0" && subOpcionSelect < 3) {
      updateMenu(subOpcionSelect + 1);
    } else if (obj == "BJ" && cm == "0") {
      selectOptionMenu(subOpcionSelect +1);
    }


  } else if (opcionSelect == 1) {  // luz 01
  } else if (opcionSelect == 2) {  // soporte
  }
}