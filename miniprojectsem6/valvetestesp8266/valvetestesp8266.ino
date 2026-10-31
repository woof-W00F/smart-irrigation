//ESP8266

int valve_switch_1 = 16;
int valve_switch_2 = 4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(valve_switch_1,OUTPUT);
  pinMode(valve_switch_2,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(valve_switch_1,HIGH);
  delay(1000);
  digitalWrite(valve_switch_1,LOW);
  digitalWrite(valve_switch_2,HIGH);
  delay(1000);
  digitalWrite(valve_switch_2,LOW);
  delay(1000);

}
