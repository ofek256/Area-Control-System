#include <utils.cpp> // load the utilities file which includes useful functions we wrote

long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float humidity = 0;
const int ledPin = 4;

void setup() {
  init();
  pinMode(ledPin, OUTPUT);
}

boolbool
void loop() {
  cnctLoop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    temperature = 5.3; 
    // Uncomment the next line to set temperature in Fahrenheit 
    // (and comment the previous temperature line)
    //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);

    humidity = 5.3; // kekw
    
    // Convert the value to a char array
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("esp32/humidity", humString);
  }
}