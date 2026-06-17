void b_warnOkRls(void *ptr){
  buzzer();
  t_warnB1.setText(kosong);
  t_warnB2.setText(kosong);
  t_warnB3.setText(kosong);
  t_warnB4.setText(kosong);
  t_warnB5.setText(kosong);
  t_warnB6.setText(kosong);
  flag_pageWarn=0;
  flag_pageMain=16;
  pageMain.show();
}
void pageWarnPop(void *ptr){
  // flagStart = false;
}