# ifndef GAME_H
# define GAME_H

# include "player.h"

typedef struct _Game
{
    Player_t* players_ptr[ROLE_NUM];  // 最多四个人 in player.c
    int player_num; // 玩家数量 [2~4]
    int init_money; // 玩家初始资金，在程序开始时根据输入初始化初始化

    int curr_player_id; // 当前玩家id，初始化为0;
} Game_t;

# endif