#include <utils.h>
#include <ldr_and_ultrasonic_func.h>
#define trigPin 5
#define echoPin 18

long distance, duration, sum=0;
int numberOfSamples = 10, counter=0;
int wasopen, state, changed, count=0;
const int door = 50;
double ave;

void setup()
{
  stpLoop();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long getDistance() //הפונקציה שמודדת מרחק
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

void loop()
{
  cnctLoop();
  while(counter < numberOfSamples) //מזמנת את הפונקציה 10 פעמים בשביל לעשות ממוצע ולקבל תוצאה הגיונית ומדויקת
  {
    getDistance();
    if(distance>3 && distance <300)
    {
      sum+=distance;
      counter++;
      delay(77);
    }
  }
  
  // חישוב המרחק הממוצע והדפסתו
  ave= sum*1.0/counter;
  Serial.print("average distance[cm] = ");
  Serial.println(ave);

//.בודקים אם הדלת פתוחה או סגורה
  if (ave>door)
    state=1;
  else
    state=0;

 //בודק אם השתנה המצב של הדלת. עובד רק אם זו המדידה השנייה והלאה, כי אחרת לא ידוע לנו המצב הקודם
  changed= status_chage_checking(count, wasopen, state);
    
  if (changed==1)
  {
    {
      Serial.println("door is open");
      client.publish("esp32/Ultrasonic", "Ultrasonic (Entrance): The door is open.");
    }
  }
  if (changed==0)
  {
    {
      Serial.println("door is closed");
      client.publish("esp32/Ultrasonic", "Ultrasonic (Entrance): The door is closed.");
    }
  }
  
  delay(7000);
  counter = 0;
  sum=0;
  count=1;
  // wasopen מסמל את המצב של הדלת לפי המדידה הקודמת. בסוף כל מדידה הוא שווה למה שהתקבל באותה מדידה
  wasopen=state;
}

