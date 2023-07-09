# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "utils.h"

extern const char land_symbol[LAND_TYPE_NUM];
extern const char role_symbol[ROLE_NUM];
const char* color_table[COLOR_NUM] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[0m"};
const char item_symbol[ITEM_NUM] = {' ', '#', ' ',  'F'};
// 使用方法：color_table[RED]获取字符串"\033[31m"
extern const Color_enum role_color[ROLE_NUM];
char info_buf[INPUT_BUFFER_SIZE];

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

void func_concat_info(char* info)
{
    if (info_buf[0] == '\0'){
        sprintf(info_buf, "%s", info);
    }else{
        strcat(info_buf, "|");        
        strcat(info_buf, info);        
    }
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

        if (func_check_some_one_here(game_ptr, i))
        {
            Role_enum role = func_tail(land_ptr[i]->privilige_role);
            land_ptr[i]->symbol = role_symbol[role];
            land_ptr[i]->color = role_color[role];
        }
    }

    // 当前玩家优先显示
    land_ptr[player_ptr[player_id]->pos]->symbol = role_symbol[player_ptr[player_id]->role];
    land_ptr[player_ptr[player_id]->pos]->color = role_color[player_ptr[player_id]->role];
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
    for (int i = 1; i < GIFT_POS-TOOL_POS; ++i)
    {   
        char symbol = land_ptr[MAGIC_PARK_POS+7-i]->symbol;
        Color_enum color = land_ptr[MAGIC_PARK_POS+7-i]->color;
        func_print_with_color(color, symbol);
        printf(SPACE_STR);
        symbol = land_ptr[i+TOOL_POS]->symbol;
        color = land_ptr[i+TOOL_POS]->color;
        func_print_with_color(color, symbol);
        putchar('\n');
    }

    // 打印最后一行
    for (int i = MAGIC_PARK_POS; i >= GIFT_POS; --i)
    {
        char symbol = land_ptr[i]->symbol;
        Color_enum color = land_ptr[i]->color;
        func_print_with_color(color, symbol);
    }
    putchar('\n');
}

void func_display_with_info(Game_t* game_ptr, int player_id, char* info)
{
    // clear
    #ifdef __linux__
    system("clear");
    #else
    system("cls");
    #endif

    Player_t* player_ptr = game_ptr->players_ptr[player_id];

    // 显示角色状态栏
    printf("[%c：回合%d|位置%d|金钱%d|点数%d|路障%d|机器娃娃%d|财神祝福%d|空地%d|茅屋%d|房子%d|摩天楼%d]\n", role_symbol[player_ptr->role], game_ptr->round_num+1, player_ptr->pos, player_ptr->money, player_ptr->point, player_ptr->barrier_cnt, player_ptr->robot_cnt, player_ptr->free_of_toll_cnt, player_ptr->solid_property_cnt[VOID_LAND], player_ptr->solid_property_cnt[HUT], player_ptr->solid_property_cnt[HOUSE], player_ptr->solid_property_cnt[SKYCRAPER]);

    if (info != NULL){
        printf("[%s]\n", info);
    }else{
        printf("[]\n");
    }

    // 刷新信息
    info_buf[0] = '\0';

    // 显示地图
    func_display_map(game_ptr, player_id);
}