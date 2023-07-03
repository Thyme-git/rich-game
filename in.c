# include <stdio.h>
# include "utils.h"

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

char *func_scanf_str(char buf[])
{
    char c;
    int i=0;
    while((c = getchar())!='\n')
    {
        if(c != ' ')
        {
            buf[i]=c;
            i++;
        }
    }
    buf[i] = '\0';
    char *str = buf;
    return str;
}

int sizeof_num(int a)
{
    if(a>69||a<-69)
    return 0;
    if(a>=0&&a<10)
    return 1;
    else if (a>=10) return 2;
    else if(a<0&&a>-10) return 2;
    else  return 3;
}