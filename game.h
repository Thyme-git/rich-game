# ifndef GAME_H
# define GAME_H

# include "land.h"
# include "player.h"

# define DEFAULT_INIT_MONEY 10000
# define MAX_INIT_MONEY 50000
# define MIN_INIT_MONEY 50000
# define RECOVERY_TIME 3
# define PRISON_TIME 2

typedef struct _Game
{
    Player_t* players_ptr[ROLE_NUM];  // 最多四个人 in player.c
    int player_num; // 玩家数量 [2~4]
    int init_money; // 玩家初始资金，在程序开始时根据输入初始化初始化

    int curr_player_id; // 当前玩家id，初始化为0;考虑去除，可能用不到

    Land_t* land_ptr[LAND_NUM]; // 游戏的地图
} Game_t;


/**
 * @brief
 * 使用方法
 * Game_t* game_ptr = NULL;
 * game_ptr = func_init_game();
 */
Game_t* func_init_game();

/**
 * @brief 
 * 使用方法
 * int done = func_game_step(game_ptr)
 * 运行一回合，即所有可以动的玩家进行一次投骰子
 * 当返回为1时，游戏结束
 */
int func_game_step(Game_t* game_ptr)

# endif