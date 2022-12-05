#include <utils.h>

#define trigPin 5
#define echoPin 18

long duration;
long distance;
int numberOfSamples = 10, counter=0;
long sum=0; 
const int door = 50;
int open;
double ave;
int wasopen;
int count=0;
int fucku;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

long getDistance();

void loop()
{

  while(counter <numberOfSamples)
  {
    getDistance();
    if(distance>3 && distance <300)
    {
      sum+=distance;
      counter++;
    }
  }
  
  ave= sum*1.0/counter;
  Serial.print("average distance[cm] = ");
  Serial.println(ave);
  delay(1000);

  if (ave>door)
  {
    Serial.println("ave>door, open should be 1");
    fucku=1;

    Serial.print("fucku= ");
    Serial.println(fucku);
  }
  else
  {
    Serial.println("ave<=door, open should be 0");
    fucku=0;

    Serial.print("fucku= ");
    Serial.println(fucku);
  }

  Serial.print("count= ");
  Serial.println(count);
  Serial.print("fucku= ");
  Serial.println(fucku);

  if (count>0)
  {
    Serial.println("fuck my life(:");
    if (fucku!=wasopen)
    {
      Serial.print("The door is ");
      if (fucku==1)
      {
        Serial.println("open");
      } 
      else
      {
        Serial.println("closed");
      }
    }


  if (count==0)
  {
    if (fucku==1)
    {
      Serial.println("the door is open");
    } 
    if (fucku==0)
    {
      Serial.println("the door is closed");
    }
  }
  
  
  delay(7000);
  counter = 0;
  sum=0;
  count=1;
  wasopen=fucku;
  Serial.print("fucku= ");
  Serial.println(fucku);
  Serial.print("wasopen= ");
  Serial.println(wasopen);
}
}


long getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin,HIGH);
  distance = duration/58.1;
  return distance;
 
}


