#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#include<DHT.h>
#include<WiFi.h>
#include<BlynkSimpleEsp32.h>

char ssid[]="The WIFI";
char password[]="123madhu";

// Pins
int dhtPin=23;
int irPin=22;
int relayPin=4;
int gasPin=34;
int buzzerPin=16;

// Actions
#define OBJECT_DETECTED 0
#define GAS_THRESHOLD 1200

// DHT Object
DHT dht(dhtPin,DHT11);

void setup() {
  projectInit();
}

void loop() {
  float h=getHumidity();
  float t=getTemperature();
  float i=getIR();
  float g=getGas();
  
  
  if(isnan(h)||isnan(t) || isnan(i) || isnan(g)) 
    return;

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(",Temperature: ");
  Serial.print(t);
  Serial.print(",IR: ");
  Serial.print(i);
  Serial.print(",Gas: ");
  Serial.println(g);

  uploadToBlynk(h,t,i,g);
  
  if(i==OBJECT_DETECTED)
    relayControl("on");
  else
    relayControl("off");

  if(g<GAS_THRESHOLD) {
    buzzerControl("on");
  } else {
    buzzerControl("off");
  }
  delay(2000);
}

void projectInit(){
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,password);

  pinMode(relayPin,OUTPUT);
  pinMode(irPin,INPUT);
  pinMode(buzzerPin,OUTPUT);
}

float getHumidity(){
  return dht.readHumidity();
}

float getTemperature(){
  return dht.readTemperature();
}

float getIR(){
  return digitalRead(irPin);
}

float getGas(){
  return analogRead(gasPin);
}

void relayControl(String action){
  if(action=="on"){
    digitalWrite(relayPin,1);
    Serial.println("AC ON");
  }
  else if(action=="off"){
    digitalWrite(relayPin,0);
    Serial.println("AC OFF");
  }
}

void buzzerControl(String action){
  if(action=="on"){
    digitalWrite(buzzerPin,1);
    Serial.println("Buzzer ON");
  }
  else if(action=="off"){
    digitalWrite(buzzerPin,0);
    Serial.println("Buzzer OFF");
  }
}

void uploadToBlynk(float h,float t,float i,float g){
  Blynk.virtualWrite(V0,h);
  Blynk.virtualWrite(V1,t);
  Blynk.virtualWrite(V2,i);
  Blynk.virtualWrite(V3,g);
  delay(10);
}
