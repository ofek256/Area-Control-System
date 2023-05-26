// This file in now deprecated and no longer in use. Function has been transferred to ldr_and_pms.h.

const int dark = 550; // const that defines the value which the lights are off below. for LDR.
#define ldr_pin 32

void ldr()
{
    Serial.println("ldr value: " + analogRead(ldr_pin));
    if (analogRead(ldr_pin) > dark)
    {
        client.publish("esp32/LDR", "LDR (FRC/Workshop): Lights on. People are present.");
        Serial.println("Lights are on. people in FRC/workshop.");
    }
    else
    {
        client.publish("esp32/LDR", "LDR (FRC/Workshop): Lights off. People are not present.");
        Serial.println("Lights are off. no people in FRC/workshop.");
    }
    delay(2000);
}