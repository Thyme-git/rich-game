# include <stdio.h>
# include "utils.h"

extern const char land_symbol[LAND_TYPE_NUM];
extern const char role_symbol[ROLE_NUM];
const char* color_table[COLOR_NUM] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[0m"};
const char item_symbol[ITEM_NUM] = {' ', '#', ' ',  '@'};
// 使用方法：color_table[RED]获取字符串"\033[31m"

const char* role_hint[ROLE_NUM] = {
    "\033[31m钱夫人>\033[0m",
    "\033[32m阿土伯>\033[0m",
    "\033[34m孙小美>\033[0m",
    "\033[33m金贝贝>\033[0m"
};

void func_print_hint(Role_enum role)
{
    printf("%s ", role_hint[role]);
}

void func_print_with_color(Color_enum color, char chr)
{
    printf("%s%c\033[0m", color_table[color], chr);
}

void func_update_map_info(Game_t* game_ptr, int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    int player_num = game_ptr->player_num;
    
    // 更新基础地形以及道具信息
    for (int i = 0; i < LAND_NUM; ++i)
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

        if (land_ptr[i]->item != VOID_ITEM)
        {
            land_ptr[i]->symbol = item_symbol[land_ptr[i]->item];
            land_ptr[i]->color = WHITE;
        }
    }

    // 找出人的位置，这里有一个优先级
    for (int i = 0; i < player_num; ++i)
    {
        int id = (player_id+1+i) % player_num;
        if (player_ptr[id]->lose)
        {
            continue;
        }
        int pos = player_ptr[id]->pos;
        char symbol = role_symbol[player_ptr[id]->role];
        land_ptr[pos]->symbol = symbol;
        land_ptr[pos]->color = player_ptr[id]->color;
    }

}


void func_display_map(Game_t* game_ptr, int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    
    // 跟新地图信息
    func_update_map_info(game_ptr, player_id);

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
