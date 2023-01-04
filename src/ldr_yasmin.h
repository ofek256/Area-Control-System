#include <utils.h>
#include <PMserial.h>
#define ldr_pin 32
int ldrnow;
const int dark = 450;
int check;
int count=0;
int old=0;
int changed;
SerialPM pms(PMS7003, 23, 19);
void setup()
{
    pinMode(ldr_pin, INPUT);
    Serial.begin(115200);
    pms.init();
    Serial.print("ldr rewstdyfugiuvalue: ");
}
void loop()
{
    pms.read();
    Serial.print(F("PM1.0 "));Serial.print(pms.pm01);Serial.print(F(", "));
    Serial.print(F("PM2.5 "));Serial.print(pms.pm25);Serial.print(F(", "));
    Serial.print(F("PM10 ")) ;Serial.print(pms.pm10);Serial.println(F(" [ug/m3]"));
    ldrnow=analogRead(ldr_pin);
    Serial.print("ldr value: ");
    Serial.println  (ldrnow);
    if (ldrnow>dark)
    {
        check=1;
    }
    else
    {
        check=0;
    }

    changed= status_chage_checking(count, old, check);
    
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
    old = check;
}