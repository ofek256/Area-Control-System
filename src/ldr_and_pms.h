#include <utils.h>
#include <pms_yasmin.h>

const int dark = 550; // const that defines the value which the lights are off below. for LDR.

#define ldr_pin 32
#define TXD2 17 // pin TX2 on the esp to sensor RXD
#define RXD2 16 // pin RX2 on the esp to sensor TXD
#define buzzer 18

void setup()
{
    stpLoop(); // Begin wifi and serial connection - see utils file
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // begin communication with PMS
    pinMode(ldr_pin, INPUT);
    pinMode(buzzer, OUTPUT);
}
void loop()
{
    cnctLoop(); // maintain wifi and serial connection - see utils file
    ldr();

    while (!com)
    {
        pms_func();
    }
    if (com)
    {
        com = !com;
    }
}

void ldr() // func for reading the LDR values and sending info to the Pi via MQTT
{
    Serial.println("ldr value: " + analogRead(ldr_pin)); // read and print the ldr's value
    if (analogRead(ldr_pin) > dark) // send the info (lights are on or lights are off)
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