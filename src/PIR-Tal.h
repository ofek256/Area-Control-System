#include <utils.h> // load the utilities file which includes useful functions we wrote

#define Sensor 18
#define LED 19 //for debugging, removed in final version

void setup() {
  stpLoop(); //Begin wifi and serial connection - see utils file
  pinMode (Sensor, INPUT); 
  pinMode (LED, OUTPUT);   
}

void loop() {
 cnctLoop(); //Connection sequence - (see utils file)
     if (digitalRead(Sensor)) { 
        digitalWrite(LED, HIGH); //for debugging, removed in final version
        Serial.println("Motion Detected");
        client.publish("esp32/PIR", "PIR (Media): Presence Detected.");
     }
     else {
        digitalWrite(LED, LOW); //for debugging, removed in final version
        Serial.println("No Motion"); 
        client.publish("esp32/PIR", "PIR (Media): No Presence.");
     }
     delay(3000);
}