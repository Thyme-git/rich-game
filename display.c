# include <stdio.h>
# include "utils.h"

extern const char land_symbol[LAND_TYPE_NUM];
extern const char role_symbol[ROLE_NUM];
const char* color_table[COLOR_NUM] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[0m"};
// 使用方法：color_table[RED]获取字符串"\033[31m"

const char* role_hint[ROLE_NUM] = {
    "\033[31m钱夫人>\033[0m",
    "\033[32m阿土伯>\033[0m",
    "\033[34m孙小美>\033[0m",
    "\033[33m金贝贝>\033[0m"
};

void func_print_hint(Role_enum role)
{
    printf("%s", role_hint[role]);
}

// void func_print_help()
// {
//     printf("\n帮助文档：\n");
//     printf("命令                       功能说明                        参数说明\n");
//     printf("\n");
//     printf("Roll            掷骰子命令，行走1`6步，步数随机产生             无\n");
//     printf("\n");
//     printf("Sell n          轮到玩家时，可出售自己的任意房产，出售     n为房产的绝对位置\n");
//     printf("                价格为投资总成本的2倍\n");
//     printf("\n");
//     printf("Block n         玩家可将路障放置到当前位置前后10部的任     n为前后相对距离\n");
//     printf("                何位置，任一玩家经过将被拦截，一次有效     负数表示后方\n");
//     printf("\n");
//     printf("Robot           使用该道具将清除前方10步内任何其他道具\n");
//     printf("\n");
//     printf("Query           显示自家资产\n");
//     printf("\n");
//     printf("Help            查看命令帮助\n");
//     printf("\n");
//     printf("Quit            强制退出\n\n");
// }

void func_print_with_color(Color_enum color, char chr)
{
    printf("%s%c\033[0m", color_table[color], chr);
}

void func_display_map(Game_t* game_ptr)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    int player_num = game_ptr->player_num;
    // 确定打印的内容
    for (int i = 0; i <= LAND_NUM; ++i)
    {
        Land_enum type = land_ptr[i]->type;
        int owner_id = land_ptr[i]->owner_id;
        Color_enum color = WHITE;
        if (owner_id != -1)
        {
            color = player_ptr[owner_id]->color;
        }
        // func_print_with_color(color, land_symbol[type]);
        land_ptr[i]->symbol = land_symbol[type];
        land_ptr[i]->color = color;
    }

    // 找出人的位置
    for (int i = 0; i < player_num; ++i)
    {
        int pos = player_ptr[i]->pos;
        char symbol = role_symbol[player_ptr[i]->role];
        land_ptr[pos]->symbol = symbol;
    }

    // 打印第一行
    for (int i = 0; i <= TOOL_POS; ++i)
    {
        char symbol = land_ptr[i]->symbol;
        Color_enum color = land_ptr[i]->color;
        func_print_with_color(color, symbol);
    }
    putchar('\n');

    // 打印中间部分
    for (int i = TOOL_POS+1; i < GIFT_POS; ++i)
    {    
        printf("$"SPACE_STR);
        char symbol = land_ptr[i]->symbol;
        Color_enum color = land_ptr[i]->color;
        func_print_with_color(color, symbol);
        putchar('\n');
    }

    // 打印最后一行
    for (int i = MAGIC_POS; i >= GIFT_POS; --i)
    {
        char symbol = land_ptr[i]->symbol;
        Color_enum color = land_ptr[i]->color;
        func_print_with_color(color, symbol);
    }
    putchar('\n');
}