#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* ssid = "REPLACE_WITH_YOUR_SSID"; // wifi network name
const char* password = "REPLACE_WITH_YOUR_PASSWORD"; // wifi network pass

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";

WiFiClient espClient;
PubSubClient client(espClient);

void init() { //void setup function with all the wifi and communications stuff
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // write dots until connected
  }
  client.setServer(mqtt_server, 1883); // setup mqtt connection

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
    if (client.connect("ESP8266Client")) {
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

void sendData(bool status, int pms, char* topic) {
    char pubString[2];
    pubString[1] = status?1:0;
    pubString[2] = (char)pms;
    client.publish(topic, pubString);
}

void callback(char* topic, byte* message, unsigned int length) { // this is if we need to recieve data with an arduino.
  Serial.print("Message arrived on topic: ");                    // we most likely wont so this wont be used.
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
/*
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    } 
  } */
}