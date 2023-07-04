# include <regex.h>
# include <stdio.h>
# include <string.h>
# include <malloc.h>
# include <stdlib.h>
# include "utils.h"

/*
typedef enum _Order
{
    ORDER_ROLL    
,   ORDER_ROBOT   
,   ORDER_QUERY   
,   ORDER_HELP    
,   ORDER_QUIT    
,   ORDER_STEP    
,   ORDER_SELL    
,   ORDER_BLOCK   
,   ORDER_MONEY   
,   ORDER_BUFF    
,   ORDER_POINT   
,   ORDER_STOP    
,   ORDER_POS     
,   ORDER_ITEM    
,   ORDER_MAP     
,   ORDER_UNMAP
,   ORDER_BOMB
,   ORDER_BARRIER
}Order_enum;
*/

const char* regex[REGEX_NUM] =
{
    "^[ ]*roll *$"
,   "^[ ]*robot *$"
,   "^[ ]*query *$"
,   "^[ ]*help *$"
,   "^[ ]*quit *$"
,   "^[ ]*step[ ]*[0-9]+ *$"
,   "^[ ]*sell[ ]*[0-9]+ *$"
,   "^[ ]*block[ ]*[-]?[0-9]+ *$"
,   "^[ ]*set[ ]*money[ ]*(A|Q|J|S)[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*buff[ ]*(A|Q|J|S)[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*point[ ]*(A|Q|J|S)[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*stop[ ]*(A|Q|J|S)[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*pos[ ]*(A|Q|J|S)[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*item[ ]*[0-9]+[ ]*(A|Q|J|S)[ ][0-9]+ *$"
,   "^[ ]*set[ ]*map[ ]*(A|Q|J|S)[ ]*[0-9]+[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*unmap[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*bomb[ ]*[0-9]+ *$"
,   "^[ ]*set[ ]*barrier[ ]*[0-9]+ *$"
};
// 使用方法regex[ORDER_ROLL] 获取 "^[ ]*roll *$"

const Reg_t* reg_ptr[REGEX_NUM] = {NULL};


Reg_t* func_init_reg(const char* regex)
{
    Reg_t* reg_ptr  = (Reg_t*)malloc(sizeof(Reg_t));

    strcpy(reg_ptr->regex, regex);

    if (regcomp(&reg_ptr->reg, reg_ptr->regex, REG_EXTENDED | REG_ICASE))
    {
        fprintf(stderr, "正则表达式编译失败！\n");
        exit(-1);
    }

    return reg_ptr;
}

/**
 * @brief 
 * 
 * @param reg_ptr 
 * @param str 
 * @return 1表示成功，0表示匹配失败 
 */
int func_reg_match(Reg_t* reg_ptr, const char* str)
{
    return regexec(&reg_ptr->reg, str, 1, &reg_ptr->reg_match, 0) == 0;
}

void func_free_reg(Reg_t* reg_ptr)
{
    regfree(&reg_ptr->reg);
    free(reg_ptr);
}

void func_init_all_reg()
{
    for (int i = 0; i < REGEX_NUM; ++i)
    {
        reg_ptr[i] = func_init_reg(regex[i]);
    }
}

/**
 * @brief 
 * 
 * @param str 
 * @return 返回-1代表错误，返回>=0代表 
 */
Order_enum func_match_order(const char* str)
{
    for (int i = 0; i < REGEX_NUM; ++i)
    {
        if (func_reg_match(reg_ptr[i], regex[i]))
        {
            return i;
        }
    }
    return -1;
}
