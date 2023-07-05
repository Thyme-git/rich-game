# include <stdio.h>
# include "utils.h"

extern const char* get_num_regex;
extern const char* get_yes_no_regex;

int scanf_num()
{
    char buf[INPUT_BUFFER_SIZE];
    int ret = 0;
    Reg_t* reg_ptr = func_init_reg(get_num_regex);
    //get_num_regex : "^[ ]*(0|[1-9]+[0-9]*)[ ]*$";
    
    func_scanf_str(buf);
    ret = func_reg_match(reg_ptr, buf);
    if (!ret)
    {
        if (buf[0] == '\0') // 输入了空指令
        {
            return -1;
        }
        return 0;
    }
    
    int sum = 0;
    for (int i = 0; buf[i] != '\0'; ++i)
    {
        if (buf[i] == ' ')
        {
            continue;
        }
        sum *= 10;
        sum += buf[i]-'0';
    }

    func_free_reg(reg_ptr);

    return sum;
}

/**
 * @brief 
 * @return 1表示用户选择了y,0表示用户选择了n
 */
int func_check_yes_or_no(const char* hint, Role_enum role)
{
    char buf[INPUT_BUFFER_SIZE] = {0};
    int ret = 0;
    Reg_t* reg_ptr = func_init_reg(get_yes_no_regex);

    do
    {
        func_print_hint(role);
        if (buf[0] == '\0') // 输入了空指令
        {
            printf(hint);
        }else{
            printf("输入错误，请重新输入！");
        }
        printf("(Y/n):");
        func_scanf_str(buf);
        ret = func_reg_match(reg_ptr, buf);
    }while (!ret);

    for (int i = 0; buf[i] != '\0'; ++i)
    {
        if (buf[i] == 'Y' || buf[i] == 'y')
        {
            return 1;
        }
    }

    func_free_reg(reg_ptr);
    return 0;
}

void func_scanf_str(char buf[])
{
    char c;
    int i = 0;
    while((c = getchar())!='\n')
    {
        buf[i]=c;
        i++;
    }
    buf[i] = '\0';
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

char scanf_char()
{
    char c=' ';
    char b=' ';
    c=getchar();
    while(c==' '||c=='\n')
    {
        c=getchar();
    }
    b=c;
    c=getchar();
    while(c==' ')
    {
        c=getchar();
    }
    if(c=='\n')
    return b;
    else return ' ';
}