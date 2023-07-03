# include "in.h"

int scanf_num()
{
    int sum = 0;
    char c = getchar();
    
    while(c-'0'>= 0 && c-'0'<= 9)
    {
       sum *= 10;
       sum += (c-'0');
       c = getchar();
    }

    if(c == '\n')
    return sum;

    while( getchar() != '\n');
    return sum;
}