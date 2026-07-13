void b_dtInfoRls(void *ptr){

}
void b_dtInfoStartRls(void *ptr){

}
void b_dtInfoSwitchRls(void *ptr){
  buzzer();
  pageDtMc.show();
}
void pageDtInfoPop(void *ptr){
  if(flag_pageDtInfo==1){
    t_dtInfo.setText(dtMcJenis);
    t_dtInfoStart.setText(dtMcStart);
  }
}