#include <string>
#include <iostream>
    int number[11]= {1,2,3,4,5,6,7,8,9,10,11};
    struct numbers
    {
        int one, two, three;
        int four, five, six, seven, eight, nine, ten;
        int eleven;
    };
    numbers numnum;
    
void loop()
{

    int* pointerArr[11]={&(numnum.one), &(numnum.two), &(numnum.three), &(numnum.four), &(numnum.five), &(numnum.six), &(numnum.seven), &(numnum.eight), &(numnum.nine), &(numnum.ten), &(numnum.eleven)};

    for (int i=0; i<11; i++)
    {
        std::cout << "enter number";
        int x = 0;
        (std::cin) >> x;
        pointerArr[i] = 1//std::stoi(x);
    }

    bool higher[]=new bool[21];
    void check( struct numbers, int number[], void* names[])
    {
        for (int i=0; i<12; i++)
        {
            if (number[i]<=names[i])
            {
                higher[i]=true;
            }
            else
            {
                higher{i}=false;
            }
        }
    }
    for (int in=0; in<12; in++)
    {
        Console.WriteLine(higher[i] + ", ");
    }
}