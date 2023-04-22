

int ldrnow;
const int dark = 450;
int check;
int count=0;
int old=0;
int changed;
#define ldr_pin 32
void ldr_func()
{
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