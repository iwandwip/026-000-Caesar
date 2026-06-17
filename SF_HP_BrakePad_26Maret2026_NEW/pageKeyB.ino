void b_b0Rls(void *ptr){buzzer();} void b_b1Rls(void *ptr){buzzer();} void b_b2Rls(void *ptr){buzzer();} void b_b3Rls(void *ptr){buzzer();}
void b_b4Rls(void *ptr){buzzer();} void b_b5Rls(void *ptr){buzzer();} void b_b6Rls(void *ptr){buzzer();} void b_b7Rls(void *ptr){buzzer();}
void b_b8Rls(void *ptr){buzzer();} void b_b9Rls(void *ptr){buzzer();} void b_b200Rls(void *ptr){buzzer();} void b_b251Rls(void *ptr){buzzer();}

void b_b210Rls(void *ptr){
  buzzer();
  if(flagNoreg == 1){
    t_loginNoreg.getText(input_loginNoreg,sizeof(input_loginNoreg));
    if(strlen(input_loginNoreg) == 7){
      Serial.print("NOREG : ");
      Serial.println(input_loginNoreg);
      client.publish(idnama_mqtt_topic.c_str(),input_loginNoreg);
    }
    flagNoreg = 0;
  }

  if(flagLot == 1){
    t_lotWo.getText(input_lotWo,sizeof(input_lotWo));
    Serial.print("LOT WO : ");
    Serial.println(input_lotWo);
    flagLot = 0;
  }
}

void keybdBPop(void *ptr){
  buzzer();
}