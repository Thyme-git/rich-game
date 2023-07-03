# include <stdio.h>
# include <string.h>
# include "utils.h"


void get_cmd(Game_t* game_ptr, int player_id)
{
    int flag = 0;
    char buf[8];
    int num = 0;
    int steps = 0;
    while(!flag)
    {
        char p[1024];
        func_scanf_str(p);
        sscanf(p,"%4s%d",buf,&steps);
        num = sizeof_num(steps);
        if(num == 0 || (strlen(p) > strlen(buf)+num))//比较p的长度和buf+num的长度
        {
            printf("你的输入错误  请重新输入指令 按help查看指令\n");
        }
        else
        {
            if(!strcmp(buf, "help"))
            { 
                func_help();
            }
            else if(!strcmp(buf, "roll"))
            {
                func_roll(game_ptr, player_id);
                flag = 1;
            }
            else if(!strcmp(buf, "query"))
            {
                func_query(game_ptr, player_id);
            }
            else if(!strcmp(buf,"step")& & steps != 0)
            {
                func_step( game_ptr,  player_id, steps);
                flag = 1;//不确定是否要进入下一位运动
            }
            else if(!strcmp(buf, "robot"))
            {
                func_robot(game_ptr, player_id);
            }
            else if(!strcmp(buf, "quit"))
            {
                func_quit(game_ptr);
            }
            else if(!strcmp(buf, "block") && steps != 0)
            {
                func_block( game_ptr, player_id, steps);
            }
            else if(!strcmp(buf, "sell") && steps != 0)
            {
                func_sell(game_ptr, player_id, steps);
            }
            else if(!strcmp(buf, "bomb") && steps != 0)
            {
                func_bomb( game_ptr,  player_id, steps);
            }
            else{
                printf("你的输入错误  请重新输入指令 按help查看指令\n");
            }
        }
    }
}