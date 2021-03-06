#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "DHT.h"
#include "ArduinoJson.h"
#define DHTPIN 2
#define DHTTYPE DHT11
#define ssid "AUSU-2.4G"
#define pwd "#42Quk3*B-"
#define topic "OL9IA22UUE/esp8266/data"
#define mqtt_server "OL9IA22UUE.iotcloud.tencentdevices.com"
#define name "esp8266"
#define username "OL9IA22UUEesp8266;12010126;OQK5V;1664560867"
#define userpwd "0caee5543d57408f38ee096fa09e661660d4361e0039fcaa89ac88ef85881b5f;hmacsha256"
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
void WIFIconnect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,pwd);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}
void MQreconnect() {
    while (!client.connected()) {
        if (client.connect(name,username,userpwd)) {
            client.publish(topic, "hello world");
        } else {
            delay(5000);
        }
    }
}
void setup() {
//    Serial.begin(9600);
    dht.begin();
    WIFIconnect();
    client.setServer(mqtt_server,1883);
    client.connect(name,username,userpwd);
}

void loop() {
    if (!client.connected()) {
        MQreconnect();
    }
    if (WiFi.status() != WL_CONNECTED) {
        WIFIconnect();
    }
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    DynamicJsonDocument doc(50);
    doc["Temperature"] = t;
    doc["Humidity"] = h;
    String output;//e.g. {"Temperature":30.4,"Humidity":74}
    serializeJson(doc,output);
//    Serial.println(output.c_str());
    client.publish(topic,output.c_str());
    delay(1000*60*5);
}
