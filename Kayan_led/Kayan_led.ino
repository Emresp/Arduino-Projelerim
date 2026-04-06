int i;
void setup() {

  for(i=2;i<=5;i++){
    pinMode(i,OUTPUT);
}


}

void loop() {

  for(i=2;i<=5;i++){
    digitalWrite(i,LOW);
  }

  delay(500);

  for(i=2;i<=5;i++){
    digitalWrite(i,HIGH);
  }

  delay(500);

  for(i=2;i<=5;i++){
    digitalWrite(i,LOW);
  }

  delay(100);

  for(i=2;i<=5;i++){
    digitalWrite(i,HIGH);
    delay(1000);
    digitalWrite(i,LOW);
  }
}
