void b_outOkRls(void *ptr){

}
void b_outNgRls(void *ptr){

}
void b_outTotalRls(void *ptr){

}
void b_outJenisRls(void *ptr){
  buzzer();
  flag_pageOutput=1;
  pageNg.show();
  
}
void b_outPcsRls(void *ptr){
  buzzer();
  // flagOutput=1;
}

void b_outSubmitRls(void *ptr){
  buzzer();
  uint32_t ok=0, ng=0;

  n_outOk.getValue(&ok);
  n_outNg.getValue(&ng);

  if(currentLayer == 0){
    aLotOk += ok;
    aLotNg += ng;
    aLotTotal = aLotOk + aLotNg;

    n_aOutput.setValue(aLotTotal);
    n_aNg.setValue(aLotNg);

  } else {
    bLotOk += ok;
    bLotNg += ng;
    bLotTotal = bLotOk + bLotNg;

    n_bOutput.setValue(bLotTotal);
    n_bNg.setValue(bLotNg);
  }
}



// void b_outSubmitRls(void *ptr){
//   buzzer();
//   t_outJenis.getText(input_jenis, sizeof(input_jenis));
//   n_outPcs.getValue(&input_jenisPcs);
//   if(flagOutput==1){
//     if(input_jenisPcs>aLotOk) input_jenisPcs=aLotOk;
//     aLotOk = aLotOk - input_jenisPcs;
//     aLotNg = aLotNg + input_jenisPcs;
//     aLotTotal = aLotOk + aLotNg;
//   }
//   else if(flagOutput==2){
//     if(input_jenisPcs>bLotOk) input_jenisPcs=bLotOk;
//     bLotOk = bLotOk - input_jenisPcs;
//     bLotNg = bLotNg + input_jenisPcs;
//     bLotTotal = bLotOk + bLotNg;
//   }
//   t_outJenis.setText(kosong);
//   n_outPcs.setValue(0);
//   pageOutput.show();
// }
void b_outCloseRls(void *ptr){
  buzzer();
  n_outOk.setValue(0);
  n_outNg.setValue(0);
  n_outTotal.setValue(0);
  t_outJenis.setText(kosong);
  n_outPcs.setValue(0);
  if(flagOutput==1){
    flag_pageMain=11;
  }
  else if(flagOutput==2){
    flag_pageMain=12;
  }
  else if(flagOutput==3){
    flag_pageMain=13;
  }
  else if(flagOutput==4){
    flag_pageMain=14;
  }
  else if(flagOutput==5){
    flag_pageMain=15;
  }
  flagOutput=0;
  pageMain.show();

}
void pageOutputPop(void *ptr){
  // if(flag_pageOutput==1){
    // t_outJenis.setText(input_list);
    // memset(input_list, 0, sizeof(input_list));
  // }
  if(flagOutput==1){
    Serial.println(aLotOk);
    Serial.println(aLotNg);
    Serial.println(aLotTotal);

    n_outOk.setValue(aLotOk);
    n_outNg.setValue(aLotNg);
    n_outTotal.setValue(aLotTotal);
  }
  else if(flagOutput==2){

    n_outOk.setValue(bLotOk);
    n_outNg.setValue(bLotNg);
    n_outTotal.setValue(bLotTotal);
  }
  if(flag_pageOutput==1){
    t_outJenis.setText(input_ng);
    memset(input_ng, 0, sizeof(input_ng));
    flag_pageOutput=0;
  }
}