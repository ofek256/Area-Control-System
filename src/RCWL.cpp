#include <utils.cpp> // load the utilities file which includes useful functions we wrote

int Sensor = 18;  
int LED = 19;


void setup() {
  init();
  pinMode (Sensor, INPUT); 
  pinMode (LED, OUTPUT);   

}


void loop() {
  cnctLoop();
  int val = digitalRead(Sensor); //Read Pin as input
     while (val > 0)
     { 
        digitalWrite(LED, HIGH);
        Serial.println("Motion Detected");
        client.publish("esp32/RCWLsensor", "Motion Detected");
        delay(1000);
       
     }
     while (val == 0)
     {
        digitalWrite(LED, LOW);
        Serial.println("No Motion"); 
        client.publish("esp32/RCWLsensor", "No Motion");
        delay(1000);
    
     }


}