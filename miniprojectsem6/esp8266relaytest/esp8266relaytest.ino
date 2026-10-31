//ESP8266 relay test

void setup(){
  Serial.begin(9600);
  pinMode(16,OUTPUT);
  pinMode(4,OUTPUT);
  }

void loop(){
  digitalWrite(16,HIGH);
  delay(2000);
  digitalWrite(16,LOW);
  delay(2000);
  digitalWrite(4,HIGH);
  delay(2000);
  digitalWrite(4,LOW);
  delay(2000);
  
  }
