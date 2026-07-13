void b_b0aRls(void *ptr){buzzer();} void b_b1aRls(void *ptr){buzzer();} void b_b2aRls(void *ptr){buzzer();} void b_b3aRls(void *ptr){buzzer();}
void b_b4aRls(void *ptr){buzzer();} void b_b5aRls(void *ptr){buzzer();} void b_b6aRls(void *ptr){buzzer();} void b_b7aRls(void *ptr){buzzer();}
void b_b8aRls(void *ptr){buzzer();} void b_b9aRls(void *ptr){buzzer();} void b_b20aRls(void *ptr){buzzer();} void b_b21aRls(void *ptr){buzzer();}
void b_b22aRls(void *ptr){buzzer();} void b_b23aRls(void *ptr){buzzer();} void b_b24aRls(void *ptr){buzzer();} void b_b25aRls(void *ptr){buzzer();}
void b_b26aRls(void *ptr){buzzer();} void b_b27aRls(void *ptr){buzzer();} void b_b28aRls(void *ptr){buzzer();} void b_b40aRls(void *ptr){buzzer();}
void b_b41aRls(void *ptr){buzzer();} void b_b42aRls(void *ptr){buzzer();} void b_b43aRls(void *ptr){buzzer();} void b_b44aRls(void *ptr){buzzer();}
void b_b45aRls(void *ptr){buzzer();} void b_b46aRls(void *ptr){buzzer();} void b_b200aRls(void *ptr){buzzer();} void b_b220aRls(void *ptr){buzzer();}
void b_b240aRls(void *ptr){buzzer();} void b_b251aRls(void *ptr){buzzer();}

void b_b210aRls(void *ptr){
  buzzer();
  if(flagMld==1){
    t_mldKode.getText(input_mldKode, sizeof(input_mldKode));
    Serial.print("LOT MLD: ");
    Serial.println(input_mldKode);
    flagMld=0;
  }
  
  // String temp = String(input_lotWo);
  // sprintf(input_lotWo,"WG%s",temp);
  // lotWos.substring(0,10).toCharArray(aLotWo, lotWos.substring(0,10).length() + 1);
  // String temp = "WG" + String(input_lotWo);
  // temp.substring(0,12).toCharArray(input_lotWo, temp.substring(0,12).length() + 1);
  
}
void keybdAPop(void *ptr){
  // Serial.println(flagModel);
}