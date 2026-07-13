void settingLayer(){
  if(address ==1|| address ==2) jmlLayer = 2; //21,22,23,
}

void aSetBgRow(uint32_t color){
  delay(50);
  Serial.print("SET A COLOR: ");
  Serial.println(color);
  b_aLayer.Set_background_color_bco(color);
  b_aModel.Set_background_color_bco(color);
  b_aCav.Set_background_color_bco(color);
  b_aIsi.Set_background_color_bco(color);
  b_aOutput.Set_background_color_bco(color);
}
void bSetBgRow(uint32_t color){
  delay(50);
  Serial.print("SET B COLOR: ");
  Serial.println(color);
  b_bLayer.Set_background_color_bco(color);
  b_bModel.Set_background_color_bco(color);
  b_bCav.Set_background_color_bco(color);
  b_bIsi.Set_background_color_bco(color);
  b_bOutput.Set_background_color_bco(color);
}

void aSetBlack(){
  delay(50);
  b_aLayer.Set_background_color_bco(0);
  b_aLayer.Set_press_background_color_bco2(0);
  b_aLayer.Set_font_color_pco(0);
  b_aLayer.Set_press_font_color_pco2(0);
  b_aModel.Set_background_color_bco(0);
  b_aModel.Set_press_background_color_bco2(0);
  b_aModel.Set_font_color_pco(0);
  b_aModel.Set_press_font_color_pco2(0);
  b_aCav.Set_background_color_bco(0);
  b_aCav.Set_font_color_pco(0);
  b_aIsi.Set_background_color_bco(0);
  b_aIsi.Set_font_color_pco(0);
  b_aOutput.Set_background_color_bco(0);
  b_aOutput.Set_font_color_pco(0);
}

void bSetBlack(){
  delay(50);
  b_bLayer.Set_background_color_bco(0);
  b_bLayer.Set_press_background_color_bco2(0);
  b_bLayer.Set_font_color_pco(0);
  b_bLayer.Set_press_font_color_pco2(0);
  b_bModel.Set_background_color_bco(0);
  b_bModel.Set_press_background_color_bco2(0);
  b_bModel.Set_font_color_pco(0);
  b_bModel.Set_press_font_color_pco2(0);
  b_bCav.Set_background_color_bco(0);
  b_bCav.Set_font_color_pco(0);
  b_bIsi.Set_background_color_bco(0);
  b_bIsi.Set_font_color_pco(0);
  b_bOutput.Set_background_color_bco(0);
  b_bOutput.Set_font_color_pco(0);
}

void aSetNormal(){
  delay(50);
  b_aLayer.Set_background_color_bco(65535);
  b_aLayer.Set_press_background_color_bco2(396);
  b_aLayer.Set_font_color_pco(396);
  b_aLayer.Set_press_font_color_pco2(65535);
  b_aModel.Set_background_color_bco(65535);
  b_aModel.Set_press_background_color_bco2(396);
  b_aModel.Set_font_color_pco(396);
  b_aModel.Set_press_font_color_pco2(65535);
  b_aCav.Set_background_color_bco(65535);
  b_aCav.Set_font_color_pco(396);
  b_aIsi.Set_background_color_bco(65535);
  b_aIsi.Set_font_color_pco(396);
  b_aOutput.Set_background_color_bco(65535);
  b_aOutput.Set_font_color_pco(396);
}

void bSetNormal(){
  delay(50);
  b_bLayer.Set_background_color_bco(65535);
  b_bLayer.Set_press_background_color_bco2(14944);
  b_bLayer.Set_font_color_pco(14944);
  b_bLayer.Set_press_font_color_pco2(65535);
  b_bModel.Set_background_color_bco(65535);
  b_bModel.Set_press_background_color_bco2(14944);
  b_bModel.Set_font_color_pco(14944);
  b_bModel.Set_press_font_color_pco2(65535);
  b_bCav.Set_background_color_bco(65535);
  b_bCav.Set_font_color_pco(14944);
  b_bIsi.Set_background_color_bco(65535);
  b_bIsi.Set_font_color_pco(14944);
  b_bOutput.Set_background_color_bco(65535);
  b_bOutput.Set_font_color_pco(14944);
}

void machineState(){
  if(strlen(aModel) == 0);
  if(jmlLayer == 2){
    if((( *aMldKode=='\0' || *bMldKode=='\0' || *aNoreg=='\0' || *bNoreg=='\0' || (strlen(aModel)==0 && aDtLayerNo==0) || (strlen(bModel)==0 && bDtLayerNo==0)) && dtMcNo==0) && !flagStop){
      b_indikator.Set_background_color_bco(63488); // MERAH
      flagStop  = true; 
      flagRun   = false; 
      flagStart = false;
    }

    if((( *aMldKode!='\0' && *bMldKode!='\0' && *aNoreg!='\0' && *bNoreg!='\0' && (strlen(aModel)>0 || aDtLayerNo>0) && (strlen(bModel)>0 || bDtLayerNo>0)) || dtMcNo>0) && !flagRun){
      b_indikator.Set_background_color_bco(2016); // HIJAU
      flagRun   = true; 
      flagStop  = false; 
      flagStart = true;
    }
  }

  else if(jmlLayer == 1){
    if((( *aMldKode=='\0' || *aNoreg=='\0' || (strlen(aModel)==0 && aDtLayerNo==0)) && dtMcNo==0) && !flagStop){
      b_indikator.Set_background_color_bco(63488);
      flagStop  = true; 
      flagRun   = false; 
      flagStart = false;
    }

    if((( *aMldKode!='\0' && *aNoreg!='\0' && (strlen(aModel)>0 || aDtLayerNo>0)) || dtMcNo>0) && !flagRun){
      b_indikator.Set_background_color_bco(2016);
      flagRun   = true; 
      flagStop  = false; 
      flagStart = true;
    }
  }
}

