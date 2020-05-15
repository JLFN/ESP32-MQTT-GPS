# ESP32-MQTT-GPS
1.Connect the VCC pin of the GPS module to the ESP32 3.3V pin.

2.Connect the GPS ground pin to the ESP32 ground pin.

3.Now, connect the RX pin of the GPS module to the TX pin of the ESP32. Similarly, 

connect the TX pin of the GPS module to the RX pin of the ESP32 (RX and TX pins of the ESP32 are defined in the software).

4.Last, connect your ESP32 to the computer through a USB cable. The ESP32 will be powered from the USB 5V.

# Libs
WiFiClient

PubSubClient

TinyGPSPlus

SoftwareSerial
# Hardware
Esp32 Dev kit and Ublox Gps module
## TODO LIST
#Make support for home-assistant https://www.home-assistant.io/integrations/mqtt_json/
