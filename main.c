# include "player.h"

extern int init_money; // 初始化资金 in player.c
extern Player_t players[ROLE_NUM]; // 最多四个人 in player.c
extern int player_num; // 玩家数量 in player.c

int main()
{
    // 考虑放在func_init_game(Game_t)函数里面
    init_money = func_input_init_money(); // 读取输入，初始化资金
    player_num = func_input_player(players); // 读取输入，初始化人物并返回玩家数量

    // 无限循环游戏
    while (1)
    {
        func_game_step(); // 一轮游戏
    }

    return 0;
}