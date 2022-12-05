#include <utils.h> // load the utilities file which includes useful functions we wrote

int Sensor = 17;  
int LED = 18;


void setup() {
//  init();
  pinMode (Sensor, INPUT); 
  pinMode (LED, OUTPUT);   
  Serial.begin(115200);

}

void loop() {
//  cnctLoop();
  int val = digitalRead(Sensor); //Read Pin as input
     if (val > 0)
     { 
        digitalWrite(LED, HIGH);
        Serial.println("Motion Detected");
//        client.publish("esp32/RCWLsensor", "Motion Detected");
        delay(1000);
       
     }
     if (val == 0)
     {
        digitalWrite(LED, LOW);
        Serial.println("No Motion"); 
//        client.publish("esp32/RCWLsensor", "No Motion");
        delay(1000);
    
     }


}