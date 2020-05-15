/*
*forked from
* MQTT and AskSensors IoT Platform
* Description: ESP32 publishes NEO-6M GPS position to AskSensors using MQTT
* Author: https://asksensors.com, 2020
* github: https://github.com/asksensors
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//include "pico.h"

//Wifi setup
const char* ssid = "WIFISSID"; // Wifi SSID
const char* password = "WIFIPASSWORD"; // Wifi Password

const unsigned int writeInterval = 25000; // write interval (in ms)
//RX2 and TX2
static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;
//MQTT server config
const char* mqtt_server = "MQTTHOST";
const char* mqttUser = "MQTTUSER";                //MQTT broker username
const char* mqttPassword = "MQTTPASS";        //MQTT broker user password
const char* pubTopic = "publish/gps/"; // publish/username/apiKeyIn
unsigned int mqtt_port = 1883;

// objects
WiFiClient askClient;
PubSubClient client(askClient);
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device

// setup
void setup() {
Serial.begin(115200);
Serial.println("*****************************************************");
Serial.println("********** Program Start : ESP32 publishes GPS position over MQTT");
Serial.print("********** connecting to WIFI : ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("->WiFi connected");
Serial.println("->IP address: ");
Serial.println(WiFi.localIP());

client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);
// GPS baud rate
ss.begin(GPSBaud);

}
// loop
void loop() {

if (!client.connected())
reconnect();
client.loop();
// This sketch displays information every time a new sentence is correctly encoded.
while (ss.available() > 0)
if (gps.encode(ss.read()))
displayInfo();

if (millis() > 5000 && gps.charsProcessed() < 10)
{
Serial.println(F("No GPS detected: check wiring."));
while(true);
}
}

// GPS displayInfo
void displayInfo() {

if (gps.location.isValid()) {
double latitude = (gps.location.lat());
double longitude = (gps.location.lng());

Serial.println("********** Publish MQTT data");
char mqtt_payload[50] = "";
snprintf (mqtt_payload, 50, "{latitude=%lf;longitude=%lf}", latitude, longitude);
Serial.print("Publish message: ");
Serial.println(mqtt_payload);
client.publish(pubTopic, mqtt_payload);
Serial.println("> MQTT data published");
Serial.println("********** End ");
Serial.println("*****************************************************");

delay(writeInterval);// delay
} else {
Serial.println(F("INVALID"));
}

}

//MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
Serial.print("Message arrived [");
Serial.print(topic);
Serial.print("] ");
for (int i = 0; i < length; i++) {
Serial.print((char)payload[i]);
}
Serial.println();
}
//MQTT reconnect
void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("********** Attempting MQTT connection...");
// Attempt to connect
if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
Serial.println("-> MQTT client connected");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println("-> try again in 5 seconds");
// Wait 5 seconds before retrying
delay(5000);
}
}
}
