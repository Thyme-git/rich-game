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
        char p[3][1024];
        int step[2] = {0};
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
            sscanf(str,"%s%d",p[0], &step[0]);
            func_step(game_ptr, player_id, step[0]);
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
            sscanf(str,"%s%d", p[0], &step[0]);
            func_block(game_ptr, player_id, step[0]);
        }
        else if(index == ORDER_SELL)
        {
            sscanf(str,"%s%d",p[0], &step[0]);
            func_sell(game_ptr, player_id, step[0]);
        }
        else if(index == ORDER_BOMB)
        {
            sscanf(str,"%s%d",p[0], &step[0]);
            func_bomb(game_ptr, player_id, step[0]);
        }
        else if(index == ORDER_MAP)
        {
            sscanf(str,"%s%s%d%s%d", p[0], p[1], &step[0], p[2], &step[1]);
            char c = p[2][0];
            func_set_map(game_ptr, c, step[0], step[1]);
        }
        else if(index == ORDER_MONEY)
        {
            sscanf(str, "%s%s%s%d", p[0], p[1], p[2], &step[0]);
            char c = p[2][0];
            func_set_money(game_ptr,c,step[0]);
        }
        else if(index == ORDER_BUFF)
        {
            sscanf(str,"%s%s%s%d",p[0],p[1],p[2],&step[0]);
            char c=p[2][0];
            func_set_buff(game_ptr,c,step[0]);
        }
        else if(index == ORDER_ITEM)
        {
            sscanf(str,"%s%s%s%d%d",p[0],p[1],p[2],&step[0],&step[1]);
            char c=p[2][0];
            func_set_item(game_ptr,c,step[0],step[1]);
        }
        else if(index == ORDER_POS)
        {
            sscanf(str,"%s%s%s%d",p[0],p[1],p[2],&step[0]);
            char c=p[2][0];
            func_set_pos(game_ptr,c,step[0]);
        }
        else if(index == ORDER_STOP)
        {
            sscanf(str,"%s%s%s%d",p[0],p[1],p[2],&step[0]);
            char c=p[2][0];
            func_set_stop(game_ptr, c, step[0]);
        }
        else if(index == ORDER_UNMAP)
        {
            sscanf(str,"%s%s%d",p[0],p[1],&step[0]);
            func_set_unmap(game_ptr,step[0]);
        }
        else if(index == ORDER_BARRIER)
        {
            sscanf(str,"%s%s%d",p[0],p[1],&step[0]);
            func_set_barrier(game_ptr, step[0]);
        }
        else if(index == ORDER_BOMB)
        {
            sscanf(str,"%s%s%d",p[0],p[1],&step[0]);
            func_set_bomb(game_ptr, step[0]);
        }
        else if(index == ORDER_POINT)
        {
            sscanf(str,"%s%s%s%d",p[0],p[1],p[2],&step[0]);
            char c=p[2][0];
            func_set_point(game_ptr,c,step[0]);
        }else if (index == ORDER_SET_BOMB)
        {
            sscanf(str,"%s%s%d",p[0],p[1],&step[0]);
            func_set_bomb(game_ptr, step[0]);
        }
    }  
}