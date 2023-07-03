# include <stddef.h>

# include "utils.h"

// extern int init_money; // 初始化资金 in player.c
// extern Player_t players[ROLE_NUM]; // 最多四个人 in player.c 考虑换成指针
// extern int player_num; // 玩家数量 in player.c

int main()
{
    Game_t* game_ptr = NULL;
    game_ptr = func_init_game();

    // 无限循环游戏
    while (func_game_step(game_ptr));

    return 0;
}