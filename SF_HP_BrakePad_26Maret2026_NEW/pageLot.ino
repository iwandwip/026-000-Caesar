void b_lotWoRls(void *ptr){
  buzzer();
  Serial.print("flagLot: ");
  Serial.println(flagLot);
  keybdB.show();
}

void b_lotWgRls(void *ptr){
  buzzer();
  keybdA.show();
}

void b_lotPcsRls(void *ptr){

}

void b_lotKuotaRls(void *ptr){}

void b_lotIsiRls(void *ptr){
  buzzer();
  keybdB.show();
}

void b_lotSubmitRls(void *ptr){
  uint32_t isi=0, cav=0;
  n_lotIsi.getValue(&isi);
  n_lotCav.getValue(&cav);
  if(currentLayer == 0){
    aLotIsi = isi;
    aMldCav = cav;
    n_aIsi.setValue(isi);
    n_aCav.setValue(cav);
    pageMain.show();
  } else {
    bLotIsi = isi;
    bMldCav = cav;
    n_bIsi.setValue(isi);
    n_bCav.setValue(cav);
    pageMain.show();
  }
}

void b_lotCloseRls(void *ptr){
  buzzer();
  pageMain.show();
}

void pageLotPop(void *ptr){
  scanNoregMode = false;
  scanLotMode   = true;
}

void b_lotOkRls(void *ptr){
  buzzer();
  char lot[15] = {0};
  t_lotWo.getText(lot, sizeof(lot));
  if(strlen(lot) != 11) return;
  strcpy(input_lotWo, lot);
  if(currentLayer == 0){
    strcpy(aLotWo, lot);
    t_aModel.setText(lot);
  } else {
    strcpy(bLotWo, lot);
    t_bModel.setText(lot);
  }
  pageMain.show();
}

