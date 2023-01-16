#include <utils.h>
/*macro definitions of PIR motion sensor pin and LED pin*/
#define PIR_MOTION_SENSOR 18//Use pin 18 to receive the signal from the module
bool state = true; // defines the last state of motion 
bool oldstate = true; 
void setup()
{
    pinMode(PIR_MOTION_SENSOR, INPUT);
    Serial.begin(115200);  
 
}
 
void loop()
{
    cnctLoop(); 
    if(digitalRead(PIR_MOTION_SENSOR))//if it detects the moving people?
    {
        Serial.println("Hi,people is coming");
        state = true; 
    }
    else
        Serial.println("Watching");
        state = false;
 delay(200);

  while(true)
     {
      if (state != oldstate) 
      {
       client.publish("esp32/PIR", "Presence Detected"); 
       Serial.println ("Presence detectad");
       
      }
      else 
      {
        client.publish("esp32/PIR", "No Motion");
        Serial.println ("motion no detecwtod");
        oldstate = state ; 
      }
}
}
