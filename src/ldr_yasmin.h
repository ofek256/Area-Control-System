#include <utils.h>
#define ldr_pin 32
int ldrnow;
const int dark = 300;
int check;
int count=0;
int old=0;
int changed;
void setup()
{
    pinMode(ldr_pin, INPUT);
    Serial.begin(115200);
    Serial.print("ldr rewstdyfugiuvalue: ");
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
    changed= status_chage_checking(count, old, ldrnow);
    if (changed==1)
    {
        Serial.println("changed!!!!!");
        Serial.println("Lights are on. people in FRC/workshop.");
    }
    if (changed==0)
    {
        Serial.println("changed!!!!!");
        Serial.println("Lights are off. no people in FRC/workshop.");
    }
    delay(3000);
    count = 1;
    old = ldrnow;
}