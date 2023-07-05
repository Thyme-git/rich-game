# include <stddef.h>
# include <stdlib.h>
# include "utils.h"

extern FILE* dump_fp;

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        dump_fp = fopen(argv[1], "w");
        if (dump_fp == NULL){
            printf("打开文件%s失败！\n", argv[1]);
            exit(-1);
        }
    }

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