#include <utils.h> // load the utilities file which includes useful functions we wrote

int Sensor = 18;  
int LED = 19;
bool state = true; // defines the last state of motion 
bool oldstate = true; 

void setup() {
  stpLoop(); //Begin wifi connection setup - (see utils file)
  Serial.begin(115200);
  pinMode (Sensor, INPUT); 
  pinMode (LED, OUTPUT);   

}

void loop() {
 cnctLoop(); //Connection sequence - (see utils file)
  int val = digitalRead(Sensor); // Read Pin as input
     if (val == 1)
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
     
      if (state != oldstate) 
      {
       if (val == 1) client.publish("esp32/RCWL", "Motion Detected");
       else if (val == 0) client.publish("esp32/RCWL", "No Motion");
      }
      oldstate = state;
     }
