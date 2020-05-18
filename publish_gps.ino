/*
*forked from
* ESP32 with ublox gps sending mqtt publish lattitude
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>

//TODO: ESP32 MQTT user config
const char* ssid = "WIFISSID"; // Wifi SSID
const char* password = "WIFIPASSWORD"; // Wifi Password

const unsigned int writeInterval = 25000; // write interval (in ms)
//RX2 and TX2
static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;
//MQTT server config
const char* mqttServer = "MQTTSERVERHOST";
const int mqttPort = 1883;
const char* mqttUser = "MQTTUSERNAME";
const char* mqttPassword = "MQTTPASSWORD";
const char* pubTopic = "publish/gps"; //


// objects
WiFiClient espClient;
PubSubClient client(espClient);
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device

// setup
void setup() {
Serial.begin(115200);
Serial.println("*****************************************************");
Serial.println("Program Start : ESP32 publishes GPS position over MQTT");
Serial.print("Connected to the WiFi network");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print("Connecting to WiFi.");
}
Serial.println("");
Serial.println("->WiFi connected");
Serial.println("->IP address: ");
Serial.println(WiFi.localIP());

client.setServer(mqttServer, mqttPort);
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
char mqtt_payload[55] = "";
snprintf (mqtt_payload, 55, "{\"latitude\": %lf,\"longitude\": %lf}", latitude, longitude);
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
