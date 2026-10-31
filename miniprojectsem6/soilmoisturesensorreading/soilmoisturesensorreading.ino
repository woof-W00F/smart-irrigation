
float reading;
int soil_sensor = 26;
void setup(){
  Serial.begin(9600);
  pinMode(soil_sensor,INPUT);

}

void loop(){
  reading = read_data();
  Serial.print("Sensor readings: ");
  Serial.print(reading);
  Serial.println("%");
  delay(1000);
  }


float read_data(){
  float val;
  val = analogRead(soil_sensor);
  val = map(val,28000,0,0,100);
  return val;
  }
