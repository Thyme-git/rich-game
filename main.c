# include "player.h"

extern int init_money; // 初始化资金 in player.c
extern Player_t players[ROLE_NUM]; // 最多四个人 in player.c 考虑换成指针
extern int player_num; // 玩家数量 in player.c

int main()
{
    Game_t* game_ptr = NULL;
    game_ptr = func_init_game();

    // 无限循环游戏
    while (1)
    {
        func_game_step(game_ptr); // 一轮游戏
    }

    return 0;
}