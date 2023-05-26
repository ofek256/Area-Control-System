#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* ssid = "Robotica"; // wifi network name kfaryarok3 Robotica
const char* password = "dcrf55rfvt66"; // wifi network pass edcr66tgvv90 dcrf55rfvt66

const char* mqtt_server = "raspberrypi.local"; // MQTT Broker (raspberry pi) IP address
const int mqtt_port = 4590; 

WiFiClient espClient;
PubSubClient client(espClient);

void stpLoop() { //void setup function with all the wifi and communications stuff
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // write dots until connected
  }
  client.setServer(mqtt_server, mqtt_port); // setup mqtt connection

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MQTT server address: ");
  Serial.println(mqtt_server); // print connection info and stuffs
}

void cnctLoop() { // basic function to keep the communication running in the loop
  if (!client.connected()) {
    // Loop until we're reconnected
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, error code: ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  }
  client.loop();
}

void callback(char* topic, byte* message, unsigned int length) { // this is if we need to recieve data with an arduino,
  Serial.print("Message arrived on topic: ");                    // we most likely wont so this wont be used
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}