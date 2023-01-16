#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* ssid = "Robotica"; // wifi network name kfaryarok3
const char* password = "dcrf55rfvt66"; // wifi network pass edcr66tgvv90

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.11";

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
  client.setServer(mqtt_server, 4590); // setup mqtt connection

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

/* void sendData(bool status, int pms, char* topic) {
    char pubString[2];
    pubString[0] = status?1:0;
    pubString[1] = (char)pms;
    client.publish(topic, pubString);
} */

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

int status_chage_checking(int count, int old, int now)
{
  int to_return;
  if (count>0)
  {
    if (now!=old) // האם השתנה המצב
    {
      if (now==1) // אם הדלת פתוחה
      {
        to_return=1;
      } 
      else // אם הדלת סגורה
      {
        to_return=0;
      }
    }
    else if (now==old)
    {
      return 4;
    }
  }

  if (count==0)//אם זו המדידה הראשונה ואין ערך של המדידה הקודמת ואני רוצה שבכל מקרה יודפס המצב הראשון
  {
    if (now==1)//האם הדלת פתוחה
    {
      to_return=1;
    } 
    if (now==0)//האם הדלת סגורה
    {
      to_return=0;
    }
  }

  return to_return;
}

