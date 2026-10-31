#include <esp_now.h>
#include <WiFi.h>

//MAC ADDRESSES
uint8_t Node1[] = {0xF4, 0x65, 0x0B, 0x4A, 0xBE, 0x8C};
uint8_t Node2[] = {0xF4, 0x65, 0x0B, 0x4A, 0x85, 0x04};
uint8_t Hub[] = {0xF4, 0x65, 0x0B, 0x49, 0x03, 0xE4};
uint8_t Valve[] = {0x2C, 0xF4, 0x32, 0x63, 0x23, 0x39};

//SETTING PIN FOR PUMP
int pump = 26;
//GLOBAL VAR TO CONTROL PUMP
bool pump_switch_for_1=0,pump_switch_for_2=0;

//Data Structure for SENSOR READINGS
typedef struct struct_message {
    int id; // must be unique for each sender board
    float sensor_readings;
} struct_message;

struct_message Data;

//Data Structure for VALVE
typedef struct valve_message{
  int id;
  bool valve_switch;
  } valve_message;

valve_message Valve_Data;

//Creating peer interface
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&Data, incomingData, sizeof(Data));
  Serial.printf("Node_ID:%u\n",Data.id);
  Serial.print("Sensor_moisture:");
  Serial.println(Data.sensor_readings);

  Valve_Data = logic(&Data);
  display_valve(&Valve_Data);
//Sending valve on/off packet
  esp_err_t result = esp_now_send(Valve, (uint8_t *) &Valve_Data, sizeof(Valve_Data));

 if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

}

void setup() {
  //Initialize Serial Monitor
  Serial.begin(9600);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

  //REGISTERING PEER WITH VALVE MODULE
  memcpy(peerInfo.peer_addr, Valve, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  //ADDING PEER
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(pump,OUTPUT);

}

void loop(){}

//IMPLEMENTING VALVE AND PUMP LOGIC
//NOTE : This only returns the valve_message since pump logic
//       was implemented at a later stage of development. So
//       for convinence pump logic was added here.
valve_message logic(struct_message *Data){
  valve_message Valve_Data;
  Valve_Data.id = Data->id;
  if (Data->sensor_readings < 30.0 ) {
      Valve_Data.valve_switch = 1;  //This sets the valve
      if (Valve_Data.id == 1) pump_switch_for_1 = 1;
      if (Valve_Data.id == 2) pump_switch_for_2 = 1;
      //digitalWrite(pump,LOW);      //This turn "ON" the pump
      } 
  else {
      Valve_Data.valve_switch = 0;  //This sets off the valve
      if (Valve_Data.id == 1) pump_switch_for_1=0;
      if (Valve_Data.id == 2) pump_switch_for_2=0;
      //if (pump_switch_for_1 && pump_switch_for_2 != 0){
        //digitalWrite(pump,HIGH);}       //This turn "OFF" the pump
      }
  if (pump_switch_for_1 || pump_switch_for_2){
      digitalWrite(pump,HIGH);
    }
  else digitalWrite(pump,LOW);
      Serial.print("Pump:");
      Serial.println(pump_switch_for_1 || pump_switch_for_2 ? "ON":"OFF");
  return Valve_Data;
  }

//DISPLAYING VALVE DATA
void display_valve(valve_message *Valve_Data){
  Serial.print("Valve_ID:");
  Serial.println(Valve_Data->id);
  Serial.print(Valve_Data->id == 1 ? "Valve_switch_1:":"Valve_switch_2:");
  Serial.println(Valve_Data->valve_switch == 1 ? "ON":"OFF");
  }
