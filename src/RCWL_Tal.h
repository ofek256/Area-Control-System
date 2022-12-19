#include <utils.h> // load the utilities file which includes useful functions we wrote

int Sensor = 18;  
int LED = 19;
bool state = true; // defines the last state of motion 
bool oldstate = true; 

void setup() {
  init();
  pinMode (Sensor, INPUT); 
  pinMode (LED, OUTPUT);   
  Serial.begin(115200);

}

void loop() {
  int val = digitalRead(Sensor); // Read Pin as input
     if (val > 0)
     { 
      state = true; 
        digitalWrite(LED, HIGH);
        Serial.println("Motion Detected");
        delay(1000);
       
     }
     if (val == 0)
     {
      state = false; 
        digitalWrite(LED, LOW);
        Serial.println("No Motion"); 
        delay(1000);
    
     }

cnctLoop();
     while(true)
     {
      if (state != oldstate) 
      {
       client.publish("esp32/RCWLsensor", "Motion Detected"); 
       
      }
      else 
      {
        client.publish("esp32/RCWLsensor", "No Motion");
        oldstate = state ; 
      }
   }
}