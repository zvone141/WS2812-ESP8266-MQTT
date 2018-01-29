#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MQTTsubscribe "ws2812b/"

const uint16_t PixelCount = 300; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

const char* ssid = "*******";
const char* password = "*******";
const char* mqtt_server = "broker.mqtt-dashboard.com";

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
RgbColor color(0, 0, 0);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  strip.Begin();
  strip.Show();
}


void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}



void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {

  String command = "";
  int R,G,B,L;
  int ind1,ind2,ind3,ind4;

  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  if(command.substring(0,4) == "RGBL"){
    
      ind1 = command.indexOf(',');  //finds location of first ,
      R = command.substring(4, ind1).toInt();   //captures first data String
      ind2 = command.indexOf(',', ind1+1 );   //finds location of second ,
      G = command.substring(ind1+1, ind2).toInt();   //captures second data String
      ind3 = command.indexOf(',', ind2+1 );
      B = command.substring(ind2+1, ind3).toInt();
      ind4 = command.indexOf(',', ind3+1 );
      L = command.substring(ind3+1).toInt(); //captures remain part of data after last ,

      RgbColor color(R, G, B);
      strip.SetPixelColor(L, color);
      }

   else if(command.substring(0,4) == "SHOW"){
      strip.Show();
   }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      client.subscribe(MQTTsubscribe);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

