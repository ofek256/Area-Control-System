#include <utils.h>

#define trigPin 5
#define echoPin 18

long duration;
long distance;
int numberOfSamples = 10, counter=0;
long sum=0; 
const int door = 50;
double ave;
int wasopen;
int count=0;
int check;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
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
  {
    check=1;
  }
  else
  {
    check=0;
  }

 //בודק אם השתנה המצב של הדלת. עובד רק אם זו המדידה השנייה והלאה, כי אחרת לא ידוע לנו המצב הקודם
  if (count>0)
  {
    if (check!=wasopen) // האם השתנה המצב
    {
      Serial.print("The door is ");
      if (check==1) // אם הדלת פתוחה
      {
        Serial.println("open");
      } 
      else // אם הדלת סגורה
      {
        Serial.println("closed");
      }
    }
  }

// במקרה שזו המדידה הראשונה, נרצה להדפיס בכל מקרה את מצב הדלת
  if (count==0)
  {
    if (check==1)
    {
      Serial.println("the door is open");
    } 
    if (check==0)
    {
      Serial.println("the door is closed");
    }
  }
  
  
  delay(7000);
  counter = 0;
  sum=0;
  count=1;
  // wasopen מסמל את המצב של הדלת לפי המדידה הקודמת. בסוף כל מדידה הוא שווה למה שהתקבל באותה מדידה
  wasopen=check;
}

