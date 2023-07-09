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
    }else{
        dump_fp = fopen("dump.out", "w");
        if (dump_fp == NULL){
            printf("打开文件dump.out失败！\n");
            exit(-1);
        }
    }

    // 初始化正则表达式
    func_init_all_reg();

    Game_t* game_ptr = NULL;
    game_ptr = func_init_game();

    // 无限循环游戏
    int done = 0;
    int round_num = 0;
    while (!done)
    {
        done = func_game_step(game_ptr, round_num++);
    }

    // close
    fclose(dump_fp);

    // 释放正则表达式
    func_free_all_reg();

    return 0;
}