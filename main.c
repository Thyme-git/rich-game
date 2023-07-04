# include <stddef.h>
# include "utils.h"

// extern int init_money; // 初始化资金 in player.c
// extern Player_t players[ROLE_NUM]; // 最多四个人 in player.c 考虑换成指针
// extern int player_num; // 玩家数量 in player.c

int main()
{
    // 初始化正则表达式
    func_init_all_reg();

    Game_t* game_ptr = NULL;
    game_ptr = func_init_game();

    // 无限循环游戏
    int done = 0;
    while (!done)
    {
        done = func_game_step(game_ptr);
    }

    // 释放正则表达式
    func_free_all_reg();

    return 0;
}