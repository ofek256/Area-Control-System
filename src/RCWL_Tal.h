#include <utils.h> // load the utilities file which includes useful functions we wrote

#define Sensor 18
#define LED 19

bool state = true; // defines the last state of motion 
bool oldstate = true; 

void setup() {
  stpLoop(); //Begin wifi and serial connection - see utils file
  pinMode (Sensor, INPUT); 
  pinMode (LED, OUTPUT);   

}

void loop() {
 cnctLoop(); //Connection sequence - (see utils file)
  int val = digitalRead(Sensor); // Read Pin as input
     if (digitalRead(Sensor)) { 
      state = true; 
        digitalWrite(LED, HIGH);
        Serial.println("Motion Detected");
        delay(1000);
     }
     else {
      state = false; 
        digitalWrite(LED, LOW);
        Serial.println("No Motion"); 
        delay(1000);
     }
     
      if (state != oldstate) 
      {
       if (val == 1) client.publish("esp32/RCWL", "RCWL (FLL): Motion Detected.");
       else if (val == 0) client.publish("esp32/RCWL", "RCWL (FLL): No Motion.");
      }
      oldstate = state;
     }
