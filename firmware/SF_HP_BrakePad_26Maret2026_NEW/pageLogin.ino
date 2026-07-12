void b_loginNoregRls(void *ptr){
  buzzer();
  flagNoreg==1;
  keybdA.show();
}

void b_loginNamaRls(void *ptr){

}

void b_loginLogoutRls(void *ptr){
  buzzer();
  input_loginNoreg[0] = '\0';
  t_loginNama.setText(kosong);
  t_loginNoreg.setText(kosong);
  pageMain.show();
}

void b_loginOkRls(void *ptr){
  buzzer();
  char nama[20] = {0};
  t_loginNama.getText(nama, sizeof(nama));
  if(strlen(nama) == 0) return;
  if(strlen(input_loginNoreg) != 7) return;
  if(currentLayer == 0){
    t_operator1.setText(nama);
  } else {
    t_operator2.setText(nama);
  }
  pageMain.show();
}

void b_loginCloseRls(void *ptr){
  buzzer();
  input_loginNoreg[0] = '\0';
  t_loginNama.setText(kosong);
  t_loginNoreg.setText(kosong);
  pageMain.show();
}

void pageLoginPop(void *ptr){
  buzzer();
  scanNoregMode = true;
  scanLotMode   = false;
  t_loginNama.setText(namaProd);
  t_loginNoreg.setText(input_loginNoreg);
}