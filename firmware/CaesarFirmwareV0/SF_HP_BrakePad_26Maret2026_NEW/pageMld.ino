void b_mldModelRls(void *ptr){}
void b_mldCavRls(void *ptr){}
void b_mldIsiRls(void *ptr){}
void b_mldClearRls(void *ptr){}
void b_mldCloseRls(void *ptr){}

void pageMldPop(void *ptr){
}

void b_mldKodeRls(void *ptr){
  keybdA.show();
}

void b_mldOkRls(void *ptr){
  buzzer();
  char kode[10];
  uint32_t cav = 0;
  t_mldKode.getText(kode, sizeof(kode));
  n_mldCav.getValue(&cav);
  if(strlen(kode) == 0) return;
  if(currentLayer == 0){
    strcpy(aMldKode, kode);
    aMldCav = cav;
    t_aModel.setText(kode);
    n_aCav.setValue(cav);
  } else {
    strcpy(bMldKode, kode);
    bMldCav = cav;
    t_bModel.setText(kode);
    n_bCav.setValue(cav);
  }
}