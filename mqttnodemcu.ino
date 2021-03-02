#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int trigPin = 0;  //D3
const int echoPin = 2;  //D4
long duration;
int distance;

const char* ssid = "Realme1";
const char* password = "12345678";
const char* mqtt_server = "iot.eclipse.org";

#define mqtt_port 1883
#define MQTT_USER "Rahul"
#define MQTT_PASSWORD "rahul"
#define MQTT "/rahul/SmartDustbin/"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup_wifi() 
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "NodeMCUClient-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) 
    {
      Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() 
{
  pinMode(trigPin, OUTPUT); 
 pinMode(echoPin, INPUT); 
  Serial.begin(115200);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}


void loop() 
{
   client.loop();
  digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
String msg = String(distance);
   char a[msg.length()];
   msg.toCharArray(a, msg.length());
client.publish(MQTT, a);
   Serial.println(distance);
}
