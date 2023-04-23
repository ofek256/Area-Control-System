#include <utils.h>
#include <ldr_yasmin.h>
#include <pms_yasmin.h>

#define ldr_pin 32
#define TXD2 17 // (pin TX2 on the esp) To sensor RXD
#define RXD2 16 // (pin RX2 on the esp) To sensor TXD
//vcc to vin, gnd to gnd.
#define buzzer 18
void setup()
{
    stpLoop();
    pinMode(ldr_pin, INPUT);
    Serial.begin(115200);
    // Set up UART connection
    // Serial2.begin(baud-rate, protocol, RX pin, TX pin);
    //SERIAL_8N1      should work as it is a common serial protocol 
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    pinMode(buzzer, OUTPUT);
}
void loop()
{
    cnctLoop();
    ldr_func();
    while(!com)
    {
        pms_func();
    }
    if (com)
    {
    com=!com;
    }
}