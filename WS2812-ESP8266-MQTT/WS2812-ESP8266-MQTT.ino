#include "FastLED.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define NUM_LEDS 300
#define DATA_PIN 3

#define MQTTsubscribe "ws2812b/"

CRGB leds[NUM_LEDS];

const char* ssid = "********";
const char* password = "********";
const char* mqtt_server = "broker.mqtt-dashboard.com";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



void setup()
{
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
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

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String command;

  for (int i = 0; i < length; i++) {
    if((char)payload[i] == 'S'){
      if(command.length() > 1) parser(command);
      FastLED.show();
      command = " ";
    }
    else{
      command += (char)payload[i];
    }
  }
  if(command.length() > 1){
      parser(command);
      command = " ";
  }
}

void parser(String command){
  int LED;
  unsigned int color;
  String barva;
  char colorChar[9];
  LED = command.substring(0,command.indexOf('x')-1).toInt();
  barva = command.substring(command.indexOf('x')+1);
  //barva.toCharArray(colorChar,9);
  //color = strtol(colorChar, NULL,16);
  
  leds[LED] = barva.toInt();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {      
      client.subscribe(MQTTsubscribe);
      
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

