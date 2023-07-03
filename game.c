# include <malloc.h>
# include <stdlib.h>
# include "game.h"
# include "in.h"

Game_t* func_init_game()
{
    Game_t* game_ptr = (Game_t*)malloc(sizeof(Game_t));
    game_ptr->player_num = 0;

    // 输入玩家初始基金
    game_ptr->init_money = DEFAULT_INIT_MONEY;
    printf("输入玩家初始资金[%d~%d](默认 %d) 或者输入回车跳过:", MIN_INIT_MONEY, MAX_INIT_MONEY, DEFAULT_INIT_MONEY);
    
    int input_money = scanf_num();
    if (input_money < MIN_INIT_MONEY || input_money > MAX_INIT_MONEY)
    {
        printf("输入范围不正确，资金初始化为默认值 %d", DEFAULT_INIT_MONEY);
    }else{
        game_ptr->init_money = input_money;
        printf("资金初始化为 %d", input_money);
    }

    // 输入角色
    printf("请选择2~4个角色：1.钱夫人 2.阿土伯 3.孙小美 4.金贝贝，可自由排序");
    char role = getchar();
    while(role != '\n')
    {
        role -= '1';
        if( role < 4 && role >= 0)
        {
            game_ptr->players_ptr[game_ptr->player_num] = func_init_player(role);
        }
        // else 
        // {   
        //     role = getchar();
        // }
        role = getchar();
        game_ptr->player_num += 1;
    }

    // 初始化Land
    func_init_land(game_ptr->land_ptr);

    return game_ptr;
}

int func_game_step(Game_t* game_ptr)
{
    Player_t* player_ptr[] = game_ptr->players_ptr;
    Land_t* land_ptr[] = game_ptr->land_ptr;
    
    for (int player_id = 0; player_id < game_ptr->player_num; ++player_id)
    {  
        // 判断存活人数
        if (func_check_game_over(game_ptr))
        {
            func_game_over(game_ptr);
        }

        // 玩家已经输了或还在医院则不执行操作
        if (player_ptr[player_id]->lose){
            continue;
        }

        if (player_ptr[player_id]->recovery_time_cnt > 0){
            player_ptr[player_id]->recovery_time_cnt -= 1;
            continue;
        }

        // 可能提示购买空地或升级 
        // 打印提示符号 
        // 读取命令并判断是否有错误
        // 执行相应的命令
        //      摇骰子
        //      可能需要收租或者被炸弹送到医院或者被拦截
        // 切换玩家

        // 提示购买或升级
        int pos = player_ptr[player_id]->pos;
        int land_type = land_ptr[pos]->type;
        if (land_type == VOID_LAND)
        {
            func_check_buy(land_ptr[pos], player_ptr[player_id]);   
        }
        if ((land_type == HUT || land_type == HOUSE) && land_ptr[pos]->owner_id == player_id)
        {
            func_check_update(land_ptr[pos], player_ptr[player_id]);
        }

        // 打印提示符号
        func_print_hint(player_ptr[player_id]->role);
        
        // 读取命令并判断是否有错误
        int re = get_cmd();
        
    }
}

void func_game_over(Game_t* game_ptr)
{
    // 释放内存
    // exit
    
}

int func_check_game_over(Game_t* game_ptr)
{
    int cnt = 0;
    for (int i = 0; i < game_ptr->player_num; ++i)
    {
        if (!game_ptr->players_ptr[i]->lose)
        {
            cnt += 1;
        }
    }

    return cnt <= 1;
}

void func_check_buy(Land_t* land_ptr, Player_t* player_ptr)
{
    printf("是否购买该空地？(Y/n)");
    char input = getchar(); // 命令解析都得重写，判断输入错误
    while (getchar() != '\n');
    if (input == 'Y')
    {
        if (player_ptr->money >= land_ptr->base_price)
        {
            player_ptr->money -= land_ptr->base_price;
            land_ptr->price += land_ptr->base_price;
            land_ptr->owner_id = player_ptr->id;
            printf("购买成功！\n");
        }else{
            printf("资金不足无法购买！\n");
        }
    }
}

void func_check_update(Land_t* land_ptr, Player_t* player_ptr)
{
    printf("是否升级该房屋？(Y/n)");
    char input = getchar(); // 命令解析都得重写，判断输入错误
    while (getchar() != '\n');
    if (input == 'Y')
    {
        if (player_ptr->money >= land_ptr->base_price)
        {
            player_ptr->money -= land_ptr->base_price;
            land_ptr->price += land_ptr->base_price;
            land_ptr->type += 1;
            printf("升级成功！\n");
        }else{
            printf("资金不足无法升级！\n");
        }
    }
}

int func_roll(Game_t* game_ptr, int player_id)
{
    int steps = rand()%6+1;
    func_player_go(game_ptr, player_id, steps);
}


/**
 * @brief 
 * player_id 在 pos 处遇到了 bomb | barrier
 * @param game_ptr 
 * @param player_id 
 * @param pos 
 */
void func_player_suffer(Game_t* game_ptr, int player_id, int pos)
{
    Land_t* land_ptr[] = game_ptr->land_ptr;
    Player_t* player_ptr[] = game_ptr->players_ptr;
    if (land_ptr[pos]->item == BOMB)
    {
        player_ptr[player_id]->recovery_time_cnt = RECOVERY_TIME;
        pos = HOSPITAL_POS;
        land_ptr[pos]->item = VOID_ITEM;
        printf("被炸伤啦，送往医院！\n");
    }
    
    // 经过路障
    if (land_ptr[pos]->item == BARRIER)
    {
        land_ptr[pos]->item = VOID_ITEM;
        printf("此路不通！\n");
    }
}

void func_player_go_prison(Game_t* game_ptr, int player_id)
{
    Land_t* land_ptr[] = game_ptr->land_ptr;
    Player_t* player_ptr[] = game_ptr->players_ptr;
    player_ptr[player_id]->recovery_time_cnt = PRISON_TIME;
}

void func_player_go(Game_t* game_ptr, int player_id, int steps)
{
    Land_t* land_ptr[] = game_ptr->land_ptr;
    Player_t* player_ptr[] = game_ptr->players_ptr;
    
    int stop = player_ptr[player_id]->pos;
    for (int i = 0; i <= steps; ++i)
    {
        // 经过炸弹
        if (land_ptr[stop]->item == BOMB)
        {
            player_ptr[player_id]->recovery_time_cnt = RECOVERY_TIME;
            stop = HOSPITAL_POS;
            land_ptr[stop]->item = VOID_ITEM;
            printf("被炸伤啦，送往医院！\n");
            break; 
        }
        
        // 经过路障
        if (land_ptr[stop]->item == BARRIER)
        {
            land_ptr[stop]->item = VOID_ITEM;
            printf("此路不通！\n");
            break;
        }
       
        stop += 1;
    }
    player_ptr[player_id]->pos = stop;
    
    // 坐牢
    if (stop == PRISON_POS)
    {
        func_player_go_prison(game_ptr, player_id);
    }
}

void func_sell(Game_t* game_ptr, int player_id, int sell_pos)
{
    if (sell_pos >= LAND_NUM || sell_pos < 0)
    {
        printf("输入的位置不存在！\n");
        return;
    }

    Land_t* land_ptr[] = game_ptr->land_ptr;
    Player_t* player_ptr[] = game_ptr->players_ptr;
    if (land_ptr[sell_pos]->owner_id != player_id)
    {
        printf("此处房产不是你的，不可售卖！\n");
        return;
    }

    player_ptr[player_id]->money += 2*land_ptr[sell_pos]->price;
    land_ptr[sell_pos]->price = 0;
    land_ptr[sell_pos]->type = VOID_LAND;
    land_ptr[sell_pos]->owner_id = -1;
}

void func_block(Game_t* game_ptr, int player_id, int block_pos)
{
    Land_t* land_ptr[] = game_ptr->land_ptr;
    Player_t* player_ptr[] = game_ptr->players_ptr;
    int pos = player_ptr[player_id]->pos;

    if (player_ptr[player_id]->barrier_cnt == 0)
    {
        printf("你没有路障可以投放！\n");
        return;
    }

    if (block_pos < -10 || block_pos > 10)
    {
        printf("投放的距离太远！\n");
        return;
    }

    if (land_ptr[pos+block_pos]->item != VOID_ITEM)
    {
        printf("此处已经有道具，不能再次放！\n");
        return;
    }

    player_ptr[player_id]->barrier_cnt -= 1;
    land_ptr[pos+block_pos]->item = BARRIER;
}

void func_bomb(Game_t* game_ptr, int player_id, int bomb_pos)
{
    Land_t* land_ptr[] = game_ptr->land_ptr;
    Player_t* player_ptr[] = game_ptr->players_ptr;
    int pos = player_ptr[player_id]->pos;

    if (player_ptr[player_id]->barrier_cnt == 0)
    {
        printf("你没有炸弹可以投放！\n");
        return;
    }

    if (bomb_pos < -10 || bomb_pos > 10)
    {
        printf("投放的距离太远！\n");
        return;
    }

    in

    if (land_ptr[pos+bomb_pos]->item != VOID_ITEM)
    {
        printf("此处已经有道具，不能再次放！\n");
        return;
    }

    player_ptr[player_id]->barrier_cnt -= 1;
    land_ptr[pos+bomb_pos]->item = BOMB;

    // 刚好炸到人
    for (int i = 0; i < game_ptr->player_num; ++i)
    {
        func_player_suffer(game_ptr, i, pos+bomb_pos);
    }
}