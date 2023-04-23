
int status_chage_checking(int count, int old, int now)
{
  int to_return;
  if (count>0)
  {
    if (now!=old) // האם השתנה המצב
    {
      if (now==1) // אם הדלת פתוחה
        to_return=1;
      else // אם הדלת סגורה
        to_return=0;

    }
    else if (now==old)

      return 4;//מסר רדומי שא ישיע ע הקוד
  }

  if (count==0)//אם זו המדידה הראשונה ואין ערך של המדידה הקודמת ואני רוצה שבכל מקרה יודפס המצב הראשון
  {
    if (now==1)//האם הדלת פתוחה
    {
      to_return=1;
    } 
    if (now==0)//האם הדלת סגורה
    {
      to_return=0;
    }
  }

  return to_return;
}