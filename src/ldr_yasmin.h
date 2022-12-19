#include <utils.h>
#define ldr_pin 19
int ldrnow;
const int dark = 200;
int check;
void setup()
{
    pinMode(ldr_pin, INPUT);
    Serial.begin(115200);
}
void loop()
{
    ldrnow=analogRead(ldr_pin);
    Serial.print("ldr value: ");
    Serial.println  (ldrnow);
    if (ldrnow>dark)
    {
        Serial.println("Lights on. People in FRC/workshop");
        check=1;
    }
    else
    {
        Serial.println("Lights off. Room is empty");
        check=0;
    }
    Serial.print("check= ");
    Serial.println  (check);
    delay(1000);
}