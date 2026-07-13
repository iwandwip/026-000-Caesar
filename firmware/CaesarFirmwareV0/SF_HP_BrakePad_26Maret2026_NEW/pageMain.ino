//===========================LAYER A===========================//
void b_iconLogin1Rls(void *ptr){
  currentLayer = 0;
  pageLogin.show();
}

void b_operator1Rls(void *ptr){
  buzzer();
  char nama[20] = {0};
  t_operator1.getText(nama, sizeof(nama));
  currentLayer = 0;
}

void b_aLayerRls(void *ptr){
  buzzer();
  flagLayer=1;
  pageDtLayer.show();
}

void b_aModelRls(void *ptr){
  buzzer();
  flagModel=1;
  pageLot.show();
}

void b_aCavRls(void *ptr){
  buzzer();
  flagCav=1;
  pageMld.show();
}
void b_aIsiRls(void *ptr){

}
void b_aOutputRls(void *ptr){
  buzzer();
  flagOutput=1;
  pageOutput.show();
}

//===========================LAYER B===========================//

void b_iconLogin2Rls(void *ptr){
  currentLayer = 1;
  pageLogin.show();
}

void b_operator2Rls(void *ptr){
  buzzer();
  char nama[20] = {0};
  t_operator2.getText(nama, sizeof(nama));
  currentLayer = 1;
}

void b_bLayerRls(void *ptr){
  buzzer();
  flagLayer=2;
  pageDtLayer.show();
}

void b_bModelRls(void *ptr){
  buzzer();
  flagModel=2;
  pageLot.show();
}
void b_bCavRls(void *ptr){
  buzzer();
  flagCav=2;
  pageMld.show();
}
void b_bIsiRls(void *ptr){

}
void b_bOutputRls(void *ptr){
  buzzer();
  flagOutput=2;
  pageOutput.show();
}

//===========================NON LAYER===========================//

void tm1TimerCallback(void *ptr){
}

void b_iconSwitchRls(void *ptr){
  buzzer();
  flagSwitch=1;
  pageDtMc.show();
}

void b_indikatorRls(void *ptr){
}

void pageMainPop(void *ptr){
  if(flag_pageMain==1){
    if(aDtLayerNo==0){
    t_aModel.setText(aModel);
    n_aIsi.setValue(aLotIsi+aLotIsi2);
    n_aOutput.setValue(aLotTotal+aLotTotal2);
    }
    if(bDtLayerNo==0){
    t_bModel.setText(bModel);
    n_bIsi.setValue(bLotIsi+bLotIsi2);
    n_bOutput.setValue(bLotTotal+bLotTotal2);
    }
    flagIsiKirim=1;
    flag_pageMain=0;
    flagCycle=0;
  }
  else if(flag_pageMain==2){
    if(aDtLayerNo==0){
    t_aModel.setText(aModel);
    n_aIsi.setValue(aLotIsi+aLotIsi2);
    n_aOutput.setValue(aLotTotal+aLotTotal2);
    }
    if(bDtLayerNo==0){
    t_bModel.setText(bModel);
    n_bIsi.setValue(bLotIsi+bLotIsi2);
    n_bOutput.setValue(bLotTotal+bLotTotal2);
    }
    flagIsiKirim=2;
    flag_pageMain=0;
    flagCycle=0;
  }
  else if(flag_pageMain==3){
    if(aDtLayerNo==0){
    t_aModel.setText(aModel);
    n_aIsi.setValue(aLotIsi+aLotIsi2);
    n_aOutput.setValue(aLotTotal+aLotTotal2);
    }
    if(bDtLayerNo==0){
    t_bModel.setText(bModel);
    n_bIsi.setValue(bLotIsi+bLotIsi2);
    n_bOutput.setValue(bLotTotal+bLotTotal2);
    }
    flagIsiKirim=3;
    flag_pageMain=0;
    flagCycle=0;
  }
  else if(flag_pageMain==4){
    if(aDtLayerNo==0){
    t_aModel.setText(aModel);
    n_aIsi.setValue(aLotIsi+aLotIsi2);
    n_aOutput.setValue(aLotTotal+aLotTotal2);
    }
    if(bDtLayerNo==0){
    t_bModel.setText(bModel);
    n_bIsi.setValue(bLotIsi+bLotIsi2);
    n_bOutput.setValue(bLotTotal+bLotTotal2);
    }
    flagIsiKirim=4;
    flag_pageMain=0;
    flagCycle=0;
  }
  else if(flag_pageMain==5){
    if(aDtLayerNo==0){
    t_aModel.setText(aModel);
    n_aIsi.setValue(aLotIsi+aLotIsi2);
    n_aOutput.setValue(aLotTotal+aLotTotal2);
    }
    if(bDtLayerNo==0){
    t_bModel.setText(bModel);
    n_bIsi.setValue(bLotIsi+bLotIsi2);
    n_bOutput.setValue(bLotTotal+bLotTotal2);
    }
    flagIsiKirim=5;
    flag_pageMain=0;
    flagCycle=0;
  }
  else if(flag_pageMain==6){
    n_aCav.setValue(aMldCav);
    // n_aIsi.setValue(aMldIsi);
    flag_pageMain=22;
  }
  else if(flag_pageMain==7){
    n_bCav.setValue(bMldCav);
    // n_bIsi.setValue(bMldIsi);
    flag_pageMain=22;
  }
  else if(flag_pageMain==11){
    n_aOutput.setValue(aLotTotal+aLotTotal2);
    flag_pageMain=0;
  }
  else if(flag_pageMain==12){
    n_bOutput.setValue(bLotTotal+bLotTotal2);
    flag_pageMain=0;
  }
  else if(flag_pageMain==16){
    delay(100);
    flag_pageMain=22;
  }
  else if(flag_pageMain==17){
    if(flagLayer==1){
      if(aDtLayerNo==0){
        sprintf(aDtLayerStart,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        aDtLayerNo=flagDtLayer;
        strcpy(aDtLayerJenis, input_dtLayer);
        flag_aDtLayerStart=1;
        delay(100);
        t_aModel.setText(aDtLayerJenis);
        aSetBgRow(63488); //MERAH
      }
    }
    else if(flagLayer==2){
      if(bDtLayerNo==0){
        sprintf(bDtLayerStart,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        bDtLayerNo=flagDtLayer;
        strcpy(bDtLayerJenis, input_dtLayer);
        flag_bDtLayerStart=1;
        delay(100);
        t_bModel.setText(bDtLayerJenis);
        bSetBgRow(63488); //MERAH
      }
    }
    memset(input_dtLayer,0,sizeof(input_dtLayer));
    flagDtLayer=0;
    flagLayer=0;
    flag_pageMain=0;
  }
  else if(flag_pageMain==18){
    if(flagLayer==1){
      if(aDtLayerNo!=0){
        sprintf(aDtLayerFinish,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        if(*aModel!='\0'){
          delay(100);
          t_aModel.setText(aModel);
        }else{
          delay(100);
          t_aModel.setText(kosong);
        }
        aSetBgRow(65535); //PUTIH
      }
    }
    else if(flagLayer==2){
      if(bDtLayerNo!=0){
        sprintf(bDtLayerFinish,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        if(*bModel!='\0'){
          delay(100);
          t_bModel.setText(bModel);
        }else{
          delay(100);
          t_bModel.setText(kosong);
        }
        bSetBgRow(65535); //PUTIH
      }
    }
    flagLayer=0;
    flag_pageMain=0;
  }

  else if(flag_pageMain==19){
    if(dtMcNo==0){
      sprintf(dtMcStart,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
      dtMcNo=flagDtMc;
      strcpy(dtMcJenis, input_dtMc);
      if(aDtLayerNo!=0){
        sprintf(aDtLayerFinish,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        flag_aDtLayer=1;
      }
      if(bDtLayerNo!=0){
        sprintf(bDtLayerFinish,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        flag_bDtLayer=1;
      }
      flagDtMcStart=1;
    }
    delay(100);
    memset(input_dtMc, 0, sizeof(input_dtMc));
    flagDtMc=0;
    flagSwitch=0;
    flag_pageDtInfo=1;
    flag_pageMain=22;
  }

  else if(flag_pageMain==20){
    if(dtMcNo!=0){
      sprintf(dtMcFinish,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());

      if(aDtLayerNo!=0){
        sprintf(aDtLayerStart,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        flag_aDtLayerStart=1;
      }
      if(bDtLayerNo!=0){
        sprintf(bDtLayerStart,"20%d-%d-%d %d:%d:%d",rtc.year(),rtc.month(),rtc.day(),rtc.hour(),rtc.minute(),rtc.second());
        flag_bDtLayerStart=1;
      }
    }
    flagDtMc=0;
    flagSwitch=0;
    flag_pageDtInfo=0;
    flag_pageMain=0;
  }
  else if(flag_pageMain==21){
    delay(200);
    flag_pageDtInfo=1;
    pageDtInfo.show();
  }
  if(flag_pageMain==22){
    flagStop = false; flagRun = false;
    machineState();
    if(dtMcNo!=0){
      flag_pageDtInfo=1;
    }
    if(flag_pageDtInfo==1){
      t_dtInfo.setText(kosong);
      t_dtInfoStart.setText(kosong);
      pageDtInfo.show();
    }
    flag_pageMain=0;
  }
}