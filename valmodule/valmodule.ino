// ESP8266
// VALVE MODULE
#include <ESP8266WiFi.h>
#include <espnow.h>

int valve_1_switch = 16; //TODO: Have to set (PS The current values are dummy) 
int valve_2_switch = 4;


typedef struct valve_message{
  int id;
  bool valve_switch;
  } valve_message;

valve_message Data;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&Data, incomingData, sizeof(Data));

  //IMPLEMENTING VALVEV ACTION LOGIC
  if (Data.id == 1){
    if (Data.valve_switch == 1) digitalWrite(valve_1_switch,HIGH);
    else digitalWrite(valve_1_switch,LOW);
  }

  if (Data.id == 2){
    if (Data.valve_switch == 1) digitalWrite(valve_2_switch,HIGH);
    else digitalWrite(valve_2_switch,LOW);
  }
  display(&Data);
}

void setup(){
  Serial.begin(9600);
  pinMode(valve_1_switch,OUTPUT);
  pinMode(valve_2_switch,OUTPUT);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  }

void loop(){delay(200);}

void display(valve_message *Data){
  Serial.print("Valve ");
  Serial.println(Data->id);
  Serial.print("Switch : ");
  Serial.println(Data->valve_switch);
  }
