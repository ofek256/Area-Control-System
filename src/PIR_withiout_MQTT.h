#include <utils.h>
/*macro definitions of PIR motion sensor pin and LED pin*/
#define PIR_MOTION_SENSOR 18 //Use pin 18 to receive the signal from the module
// bool state = true; // defines the last state of motion 
// bool oldstate = true; 
// int val;
void setup()
{
   // stpLoop(); //Begin wifi connection setup - (see utils file)
    pinMode(PIR_MOTION_SENSOR, INPUT);
    Serial.begin(115200);  
 
}
 
void loop()
{
   // cnctLoop(); // wifi connecting sequence - (see utils file)
    if(digitalRead(PIR_MOTION_SENSOR))//if it detects the moving people?
    {
        Serial.println("people are coming");
       // state = true; 
        // val == 1;
    }
    else
        Serial.println("Watching");
       // state = false;
       // val == 0;
 delay(200);




 /* if (state != oldstate) // Publish sensor status loop
      {
       if (val == 1)  client.publish("esp32/PIR", "Presence Detected"); 
       else if (val == 0) client.publish("esp32/RCWL", "No Motion");
      }
      oldstate = state; */

}