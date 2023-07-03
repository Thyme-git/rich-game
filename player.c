# include <malloc.h>
# include "utils.h"

// int init_money; // 玩家初始资金，在程序开始时根据输入初始化初始化
const char role_symbol[ROLE_NUM] = {'Q', 'A', 'S', 'J'}; // 定义角色的显示符号,使用方法 printf("%c", role_symbol[ROLE_Q]);
// Player_t players[ROLE_NUM]; // 表示（最多）四个玩家
// int player_num; // 玩家数量 [2~4]
const Color_enum role_color[ROLE_NUM] = {RED, GREEN, BLUE, YELLOW};

Player_t* func_init_player(Role_enum role, int id, int init_money)
{
    Player_t * player = (Player_t *)malloc(sizeof(Player_t *));
    player->barrier_cnt = 0;
    player->bomb_cnt = 0;
    player->color = role_color[role];
    player->role = role;
    player->money = init_money;
    player->lose = 0;
    player->point = 0;
    player->pos = 0;
    player->recovery_time_cnt = 0;
    player->robot_cnt = 0;
    player->free_of_toll_cnt = 0;
    player->id = id;
    return player;
}