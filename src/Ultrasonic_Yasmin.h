#include <utils.h>
#define trigPin 5
#define echoPin 18

long distance, duration, sum = 0;
int numberOfSamples = 10, counter = 0; // number of samples to average out
const int door = 15;                   // the distance which the door is closed when the measurement is smaller than it (in cm)
double avg;

void setup()
{
  stpLoop();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long getDistance() // func to measure distance from door
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.1;
  return distance;
}

void loop()
{
  cnctLoop();
  while (counter < numberOfSamples) // repeat the measurement multiple times to get an accurate avg
  {
    getDistance();
    if (distance > 0.5 && distance < 300) // skip invalid measurements (anything over 300cm)
    {
      sum += distance;
      counter++;
      delay(100);
    }
  }

  avg = sum / counter; // create the avg then print it
  Serial.print("avg distance[cm] = ");
  Serial.println(avg);

  sum = 0; // reset variables to prep for next loop
  counter = 0;

  if (avg >= door) // check the result and send it to the pi via MQTT
  {
    Serial.println("door is open");
    client.publish("esp32/Ultrasonic", "Ultrasonic (Entrance): The door is open.");
  }

  else
  {
    Serial.println("door is closed");
    client.publish("esp32/Ultrasonic", "Ultrasonic (Entrance): The door is closed.");
  }

  delay(2500); // delay before looping again
}