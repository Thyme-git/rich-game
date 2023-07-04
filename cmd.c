# include <stdio.h>
# include <string.h>
# include "utils.h"


void get_cmd(Game_t* game_ptr, int player_id)
{
    int flag = 0;
    while(!flag)
    {
        // 打印提示符号
        func_print_hint(game_ptr->players_ptr[player_id]->role);
        char p[1024][3];
        int step[2]={0};
        char str[1024];
        func_scanf_str(str);
        Order_enum index = func_match_order(str);
        if(index == -1)
        {
            printf("你的输入错误  请重新输入指令 按help查看指令\n");
        }
        else if(index ==  ORDER_HELP)
        { 
            func_help();
        }
        else if(index == ORDER_ROLL)
        {
            func_roll(game_ptr, player_id);
            flag = 1;
        }
        else if(index == ORDER_QUERY)
        {
            func_query(game_ptr, player_id);
        }
        else if(index == ORDER_STEP)
        {
            sscanf(str,"%*s%d",p[0],step[0]);
            func_step( game_ptr,  player_id, step[0]);
            flag = 1;
        }
        else if(index == ORDER_ROBOT)
        {
            func_robot(game_ptr, player_id);
        }
        else if(index == ORDER_QUIT)
        {
            func_quit(game_ptr);
        }
        else if(index == ORDER_BLOCK)
        {
            sscanf(str,"%s%d",p[0],step[0]);
            func_block( game_ptr,  player_id, step[0]);
        }
        else if(index == ORDER_SELL)
        {
            sscanf(str,"%s%d",p[0],step[0]);
            func_sell( game_ptr,  player_id, step[0]);
        }
        else if(index == ORDER_BOMB)
        {
            sscanf(str,"%s%d",p[0],step[0]);
            func_bomb( game_ptr,  player_id, step[0]);
        }
        else if(index == ORDER_MAP)
        {

        }
        else if(index == ORDER_MONEY)
        {

        }
        else if(index == ORDER_BUFF)
        {

        }
        else if(index == ORDER_ITEM)
        {

        }
        else if(index == ORDER_POS)
        {

        }
        else if(index == ORDER_STOP)
        {

        }
        else if(index ==ORDER_UNMAP)
        {

        }
        else if(index ==ORDER_BARRIER)
        {

        }
    }  
}