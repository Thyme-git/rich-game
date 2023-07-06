# include <stdio.h>
# include "utils.h"

extern const char* get_num_regex;
extern const char* get_yes_no_regex;
extern const char* get_item_regex;
extern const char* get_gift_regex;
extern const char* get_role_regex;

/*  
获得角色顺序，例如输入"123"获得顺序为"0 1 2 -1",前面和后面可加空格，中间不可
*/
void get_role_order(Role_enum order[])
{
    Reg_t* reg_ptr = func_init_reg(get_role_regex);
    int done = 0;
    char str[INPUT_BUFFER_SIZE];
    int check_occur = 0;
    int player_num = 0;

    for (int i = 0; i < ROLE_NUM; ++i)
    {
        order[i] = ROLE_VOID;
    }

    while (!done)
    {
        player_num = 0;
        check_occur = 0;

        printf("请选择2~4个角色：1.钱夫人 2.阿土伯 3.孙小美 4.金贝贝，可自由排序");
        func_scanf_str(str);
        done = func_reg_match(reg_ptr, str);
        if (done)
        {
            for (int i = 0; i < reg_ptr->reg_match.rm_eo; ++i)
            {
                if (str[i] == ' ')
                {
                    continue;
                }
                
                Role_enum role = str[i]-'1';
                if (check_occur & (1 << role))
                {
                    done = 0;
                    break;
                }
                check_occur |= (1 << role);
                order[player_num++] = role;
            }
        }

        if (!done)
        {
            if(!func_check_blank_input(str)){
                printf("输入错误，请重新输入\n");
            }
        }
    }
    func_free_reg(reg_ptr);
}

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
        if (func_check_blank_input(buf)) // 输入了空指令
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
    // 正则判断输入
    char buf[INPUT_BUFFER_SIZE] = {0};
    int ret = 0;
    Reg_t* reg_ptr = func_init_reg(get_yes_no_regex);
    do
    {
        func_print_hint(role);
        if (func_check_blank_input(buf)) // 输入了空指令
        {
            printf(hint);
        }else{
            printf("输入错误，请重新输入！");
        }
        printf("(Y/n):");
        func_scanf_str(buf);
        ret = func_reg_match(reg_ptr, buf);
    }while (!ret);
    func_free_reg(reg_ptr);

    for (int i = 0; buf[i] != '\0'; ++i)
    {
        if (buf[i] == 'Y' || buf[i] == 'y')
        {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief 读取用户输入选择道具
 * 
 * @return Item_enum , VOID_ITEM(0)表示退出道具屋
 */
Item_enum func_get_item(Role_enum role)
{
    char buf[INPUT_BUFFER_SIZE];
    int ret = 0;
    Reg_t* reg_ptr = func_init_reg(get_item_regex);
    func_print_hint(role);
    func_scanf_str(buf);
    while(!func_reg_match(reg_ptr, buf))
    {
        func_print_hint(role);
        if (!func_check_blank_input(buf)){ // 不是输入空指令
            printf("输入错误，请重新输入！");
        }
        func_scanf_str(buf);
    }
    func_free_reg(reg_ptr);
    
    for (int i = 0; buf[i] != '\0'; ++i)
    {
        if (buf[i] <= '3' && buf[i] >= '1')
        {
            return buf[i]-'0';
        }

        if (buf[i] == 'F' || buf[i] == 'f')
        {
            return 0;
        }
    }

    return 0;
}

/**
 * @brief 
 * 
 * @return 1 表示空指令，0表示非空指令 
 *
*/
int func_check_blank_input(char buf[])
{
    for (int i = 0; buf[i] != '\0'; ++i)
    {
        if (buf[i] != ' ')
        {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief 根据用户输入选择礼品
 * 
 * @return int -1表示放弃 1 2 3 分别表示奖金，点数，财神
 */
int func_get_gift(Role_enum role)
{
    char buf[INPUT_BUFFER_SIZE];
    int ret = 0;
    Reg_t* reg_ptr = func_init_reg(get_item_regex);
    func_print_hint(role);
    func_scanf_str(buf);
    while(!func_reg_match(reg_ptr, buf))
    {
        if (!func_check_blank_input(buf)){
            break;
        }
        func_print_hint(role);
        func_scanf_str(buf);
    }
    func_free_reg(reg_ptr);
    
    for (int i = 0; buf[i] != '\0'; ++i)
    {
        if (buf[i] <= '3' && buf[i] >= '1')
        {
            return buf[i]-'0';
        }
    }

    return -1;
}

void func_scanf_str(char buf[])
{
    char c;
    int i = 0;
    while((c = getchar()) != '\n' && c != EOF)
    {
        buf[i]=c;
        i++;
    }
    buf[i] = '\0';
}