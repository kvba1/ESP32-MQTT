#include <Arduino.h>
#include <Wifi.h>
#include <PubSubClient.h>


const char* ssid = "your WiFi ssid";
const char* password = "WiFi password";
const char* brokerUser = "name";
const char* brokerPass = "public";
const char* broker = "mqtt-broker-adress";
const char* outTopic = "topic/out";
const char* inTopic = "topic/in";
uint16_t port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;
char messages[50];

void callback(const char* topic, byte* payload, unsigned int length){
  Serial.print("Received messages: ");
  Serial.println(topic);
  for (int i=0; i<length; i++){
    Serial.print((char) payload[i]);
    if (payload[i]==1)
    {
      digitalWrite(LED_BUILTIN,HIGH);
    }
  }
  Serial.println();
}

void reconnect(){
  while(!client.connected()){
    Serial.print("\nConnecting to ");
    Serial.println(broker);
    if(client.connect("kuba",brokerUser,brokerPass)){
      Serial.print("Connected to ");
      Serial.println(broker);
      client.subscribe(inTopic);
    } else{
    Serial.print("\nTrying to connect");
    delay(5000);
  }
}
}

void wifi_connection(){

  delay(100);
  Serial.print("\nConnecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }

  Serial.print("\nConnected to ");
  Serial.print(ssid);

}

void setup() {
Serial.begin(115200);
wifi_connection();
client.setServer(broker,port);
client.setCallback(callback);
}

void loop() {
 if (!client.connected()){
  reconnect();
 } 
 client.loop();
 currentTime = millis();
 if(currentTime-lastTime >2000){
  count++;
  float d;
  sniprintf(messages,75,"Count: %ld",count);
  Serial.print(messages);
  client.publish(outTopic,messages);
  lastTime = millis();
 }
}



