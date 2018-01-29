# WS2812-ESP8266-MQTT
Controlling ws2812 led strip with esp8266 through MQTT protocol

# How to use:

Send commands via MQTT on ws2812b/ topic

Command1 = RGBL[0-255 for red],[0-255 for green],[0-255 for blue],[0- for LED position]   example: RGBL100,50,255,0
Command2 = SHOW (that will show command1 on the led strip)
