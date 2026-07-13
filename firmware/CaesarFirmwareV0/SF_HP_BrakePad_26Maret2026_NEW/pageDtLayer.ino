void pageDtLayerPop(void *ptr){

}

void b_dtLayer1Rls(void *ptr){ // BAHAN TELAT
  buzzer();
  t_dtLayer1.getText(input_dtLayer, sizeof(input_dtLayer));
  flagDtLayer = 1;
  flag_pageMain = 17;
  pageMain.show();
}

void b_dtLayer2Rls(void *ptr){ // TUNGGU PANAS
  buzzer();
  t_dtLayer2.getText(input_dtLayer, sizeof(input_dtLayer));
  // flagDtLayer = 1;
  // flag_pageMain = 17;
  pageMain.show();
} 

void b_dtLayer3Rls(void *ptr){ // MOULD OFF
  buzzer();
  t_dtLayer3.getText(input_dtLayer, sizeof(input_dtLayer));
  // flagDtLayer = 1;
  // flag_pageMain = 17;
  pageMain.show();
} 

void b_dtLayer4Rls(void *ptr){ //LEBIH PANAS
  buzzer();
  t_dtLayer3.getText(input_dtLayer, sizeof(input_dtLayer));
  // flagDtLayer = 1;
  // flag_pageMain = 17;
  pageMain.show();
} 

void b_dtLayer5Rls(void *ptr){ // KURANG PANAS
  buzzer();
  t_dtLayer3.getText(input_dtLayer, sizeof(input_dtLayer));
  // flagDtLayer = 1;
  // flag_pageMain = 17;
  pageMain.show();
} 

void b_dtLayer6Rls(void *ptr){ //TRIAL
  buzzer();
  t_dtLayer3.getText(input_dtLayer, sizeof(input_dtLayer));
  // flagDtLayer = 1;
  // flag_pageMain = 17;
  pageMain.show();
} 

void b_dtLayerRunRls(void *ptr){ // RUNNING
  buzzer();
  t_dtLayer3.getText(input_dtLayer, sizeof(input_dtLayer));
  // flagDtLayer = 1;
  // flag_pageMain = 17;
  pageMain.show();
} 

void b_dtLayerCloseRls(void *ptr){
  buzzer();
  t_dtLayer3.getText(input_dtLayer, sizeof(input_dtLayer));
  // flagDtLayer = 1;
  // flag_pageMain = 17;
  pageMain.show();
}