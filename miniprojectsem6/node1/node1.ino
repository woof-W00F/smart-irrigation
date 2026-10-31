#include <esp_now.h>
#include <WiFi.h>
//MAC ADDRESSES
uint8_t Node1[] = {0xF4, 0x65, 0x0B, 0x4A, 0xBE, 0x8C};
uint8_t Node2[] = {0xF4, 0x65, 0x0B, 0x4A, 0x85, 0x04};
uint8_t Hub[] = {0xF4, 0x65, 0x0B, 0x49, 0x03, 0xE4};

//SETTING PIN FOR SENSOR
int soil_sensor = 34;

//MESSAGE STRUCTURE
typedef struct struct_message {
    int id=0; // must be unique for each sender board
    float sensor_readings=10;
} struct_message;

//struct_message Data;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(9600);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, Hub, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

//SETTING PINMODE FOR SENSOR
}

void loop(){
  struct_message Data;
  Data.id = 1;
  Data.sensor_readings = read_data();
  Serial.print("Sensor readings: ");
  Serial.print(Data.sensor_readings);
  Serial.println("%");

  esp_err_t result = esp_now_send(Hub, (uint8_t *) &Data, sizeof(Data));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
  
  }

float read_data(){
  float val;
  val = analogRead(soil_sensor);
  Serial.println(val);
  val = map(val,4095,0,0,100);
  //val = (1-(val/28000))*100;
  return val;
  }

 
