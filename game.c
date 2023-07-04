# include <malloc.h>
# include <stdlib.h>
# include <stdlib.h> // system

# include "utils.h"

extern const char role_symbol[ROLE_NUM];

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
        printf("输入范围不正确，资金初始化为默认值 %d\n", DEFAULT_INIT_MONEY);
    }else{
        game_ptr->init_money = input_money;
        printf("资金初始化为 %d\n", input_money);
    }

    // 输入角色
    printf("请选择2~4个角色：1.钱夫人 2.阿土伯 3.孙小美 4.金贝贝，可自由排序");
    char role = getchar();
    while(role != '\n')
    {
        role -= '1';
        if( role < 4 && role >= 0)
        {
            game_ptr->players_ptr[game_ptr->player_num] = func_init_player(role, game_ptr->player_num, game_ptr->init_money);
        }
        role = getchar();
        game_ptr->player_num += 1;
    }

    // 初始化Land
    func_init_land(game_ptr->land_ptr);

    return game_ptr;
}

int func_game_step(Game_t* game_ptr)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    Land_t** land_ptr = game_ptr->land_ptr;
    
    for (int player_id = 0; player_id < game_ptr->player_num; ++player_id)
    {  
        // 判断game over
        if (func_check_game_over(game_ptr))
        {
            func_game_over(game_ptr);
            return 1;
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

        int pos = player_ptr[player_id]->pos;
        int land_type = land_ptr[pos]->type;
        
        // test
        // Player_t** player_ptr = game_ptr->players_ptr;
        // player_ptr[player_id]->point = 1000;
        // player_ptr[player_id]->bomb_cnt = 1000;

        // 道具屋
        if (pos == TOOL_POS)
        {
            func_pass_tool(game_ptr, player_id);
        }

        // 禮品屋
        if (pos == GIFT_POS)
        {
            func_pass_gift(game_ptr, player_id);
        }

        // 魔法屋
        // ...

        // clear
        system("clear");

        // 显示地图
        func_display_map(game_ptr, player_id);
       
        // 提示购买或升级
        if (land_type == VOID_LAND)
        {
            func_check_buy(land_ptr[pos], player_ptr[player_id]);   
        }
        if ((land_type == HUT || land_type == HOUSE) && land_ptr[pos]->owner_id == player_id)
        {
            func_check_update(land_ptr[pos], player_ptr[player_id]);
        }

        // 读取命令并判断是否有错误、执行
        get_cmd(game_ptr, player_id);

        // 跟新位置信息
        pos = player_ptr[player_id]->pos;
        land_type = land_ptr[pos]->type;

        // 坐牢
        if (pos == PRISON_POS)
        {
            func_player_go_prison(game_ptr, player_id);
        }

        // 減少財神次數
        if (game_ptr->players_ptr[player_id]->free_of_toll_cnt > 0)
        {
            game_ptr->players_ptr[player_id]->free_of_toll_cnt -= 1;
        }
    }
    return 0;
}

void func_free_mem(Game_t* game_ptr)
{
    // 释放内存
    for (int i = 0; i < game_ptr->player_num; ++i)
    {
        free(game_ptr->players_ptr[i]);
    }
    for (int i = 0; i < LAND_NUM; ++i)
    {
        free(game_ptr->land_ptr[i]);
    }
}

void func_game_over(Game_t* game_ptr)
{
    // 找出赢家
    int winner_id = 0;
    for (int i = 0; i < game_ptr->player_num; ++i)
    {
        if (!game_ptr->players_ptr[i]->lose)
        {
            winner_id = i;
            break;
        }
    }
    printf("玩家%d获胜啦！\n", winner_id);

    // 释放内存
    func_free_mem(game_ptr);

    // exit
    exit(0);
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
    func_print_hint(player_ptr->role);
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
    func_print_hint(player_ptr->role);
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
    func_step(game_ptr, player_id, steps);
}

/**
 * @brief 
 * player_id 在 pos 处遇到了 bomb | barrier，该函数会改变玩家的位置（受到炸弹攻击）
 * @param game_ptr 
 * @param player_id 
 * @param pos 
 * @return 1 表示玩家受道具攻击, 0表示正常
 */
int func_player_suffer(Game_t* game_ptr, int player_id, int pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;

    // 被炸伤了
    if (land_ptr[pos]->item == BOMB)
    {
        player_ptr[player_id]->pos = HOSPITAL_POS;
        player_ptr[player_id]->recovery_time_cnt = RECOVERY_TIME;
        land_ptr[pos]->item = VOID_ITEM;
        printf("被炸伤啦，送往医院！\n");
        return 1;
    }

    // 经过路障
    if (land_ptr[pos]->item == BARRIER)
    {
        player_ptr[player_id]->pos = pos;
        land_ptr[pos]->item = VOID_ITEM;
        printf("此路不通！\n");
        return 1;
    }
    return 0;
}

void func_player_go_prison(Game_t* game_ptr, int player_id)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    player_ptr[player_id]->recovery_time_cnt = PRISON_TIME;
}

void func_step(Game_t* game_ptr, int player_id, int steps)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    
    int suffer = 0;
    int stop = player_ptr[player_id]->pos;
    for (int i = 0; i < steps; ++i)
    {
        // 经过炸弹或路障
        if (func_player_suffer(game_ptr, player_id, stop)){
            suffer = 1;
            break;
        }
        stop += 1;
    }

    if (!suffer)
    {
        player_ptr[player_id]->pos += steps;
    }

    // 收租啦
    func_pay_toll(game_ptr, player_id);

    // 礦山
    func_get_point(game_ptr, player_id);
}

void func_get_point(Game_t* game_ptr, int player_id)
{
    int pos = game_ptr->players_ptr[player_id]->pos;
    game_ptr->players_ptr[player_id]->point += game_ptr->land_ptr[pos]->point;
}

void func_pay_toll(Game_t* game_ptr, int player_id)
{
    int pos = game_ptr->players_ptr[player_id]->pos;
    int owner_id = game_ptr->land_ptr[pos]->owner_id;

    // 土地沒有主人 或者 主人是自己 或者 主人在監獄中
    if (owner_id == -1 || owner_id == player_id || game_ptr->players_ptr[owner_id]->recovery_time_cnt > 0)
    {
        return;
    }

    // 財神祝福
    if (game_ptr->players_ptr[player_id]->free_of_toll_cnt > 0)
    {
        printf("財神附身，可免過路費\n");
        return;
    }

    game_ptr->players_ptr[player_id]->money -= game_ptr->land_ptr[pos]->price / 2;
    game_ptr->players_ptr[owner_id]->money += game_ptr->land_ptr[pos]->price / 2;

    // 破產
    if (game_ptr->players_ptr[player_id]->money < 0)
    {
        game_ptr->players_ptr[owner_id]->money += game_ptr->players_ptr[player_id]->money;
        func_bankrupt(game_ptr, player_id);
    }
}

void func_bankrupt(Game_t* game_ptr, int player_id)
{
    game_ptr->players_ptr[player_id]->lose = 1;
    
    Land_t** land_ptr = game_ptr->land_ptr;
    for (int i = 0; i < LAND_NUM; ++i)
    {
        if (land_ptr[i]->owner_id == player_id)
        {
            land_ptr[i]->owner_id = -1;
            // land_ptr[i]->color = WHITE;
            // land_ptr[i]->symbol = '0';
            land_ptr[i]->type = VOID_LAND;            
        }
    }
}

void func_sell(Game_t* game_ptr, int player_id, int sell_pos)
{
    if (sell_pos >= LAND_NUM || sell_pos < 0)
    {
        printf("输入的位置不存在！\n");
        return;
    }

    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
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
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
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

    int target_pos = (pos + block_pos) % LAND_NUM;
    if (target_pos < 0)
    {
        target_pos += LAND_NUM;
    }

    // 特殊位置不能放
    if (func_check_special_pos(game_ptr, target_pos))
    {
        printf("特殊位置不能投放！\n");
        return;
    }

    // 有人不能放
    if (func_check_some_one_here(game_ptr, target_pos))
    {
        printf("有玩家的位置不能投放！\n");
        return;
    }

    if (land_ptr[target_pos]->item != VOID_ITEM)
    {
        printf("此处已经有道具，不能再次放！\n");
        return;
    }

    player_ptr[player_id]->barrier_cnt -= 1;
    land_ptr[target_pos]->item = BARRIER;
}

void func_bomb(Game_t* game_ptr, int player_id, int bomb_pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    int pos = player_ptr[player_id]->pos;

    if (player_ptr[player_id]->bomb_cnt <= 0)
    {
        printf("你没有炸弹可以投放！\n");
        return;
    }

    if (bomb_pos < -10 || bomb_pos > 10)
    {
        printf("投放的距离太远！\n");
        return;
    }

    int target_pos = (pos + bomb_pos) % LAND_NUM;
    if (target_pos < 0)
    {
        target_pos += LAND_NUM;
    }

    // 特殊位置不能放
    if (func_check_special_pos(game_ptr, target_pos))
    {
        printf("特殊位置不能投放！\n");
        return;
    }

    // 有人不能放炸弹
    if (func_check_some_one_here(game_ptr, target_pos))
    {
        printf("有玩家的位置不能投放！\n");
        return;
    }

    if (land_ptr[target_pos]->item != VOID_ITEM)
    {
        printf("此处已经有道具，不能再次放！\n");
        return;
    }

    player_ptr[player_id]->bomb_cnt -= 1;
    land_ptr[target_pos]->item = BOMB;

    // 刚好炸到人
    for (int i = 0; i < game_ptr->player_num; ++i)
    {
        if (player_ptr[player_id]->pos == pos+bomb_pos){
            func_player_suffer(game_ptr, i, pos+bomb_pos);
        }
    }
}

void func_robot(Game_t* game_ptr,int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    
    int pos = player_ptr[player_id]->pos;
    if ( player_ptr[player_id]->robot_cnt <= 0)
    {
        printf("你没有机器人可以投放！\n");
        return;
    }

    for (int i = 1; i <= 10;i++)
    {
        land_ptr[(pos+i) % LAND_NUM]->item=VOID_ITEM;
    }
    return ;
}

void func_query(Game_t* game_ptr,int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;

    printf("资金:%d\n", player_ptr[player_id]->money);
    printf("点数:%d\n", player_ptr[player_id]->point);
    printf("财神祝福:%d\n", player_ptr[player_id]->free_of_toll_cnt);
    printf("路障:%d\n", player_ptr[player_id]->barrier_cnt);
    printf("炸弹:%d\n", player_ptr[player_id]->bomb_cnt);
    printf("机器娃娃:%d\n", player_ptr[player_id]->robot_cnt);
    // printf("\n");
}

void func_quit(Game_t* game_ptr)
{
    func_free_mem(game_ptr);
    exit(0);
}

void func_help()
{
    printf("\n帮助文档：\n");
    printf("命令                       功能说明                        参数说明\n");
    printf("\n");
    printf("Roll            掷骰子命令，行走1`6步，步数随机产生             无\n");
    printf("\n");
    printf("Sell n          轮到玩家时，可出售自己的任意房产，出售     n为房产的绝对位置\n");
    printf("                价格为投资总成本的2倍\n");
    printf("\n");
    printf("Block n         玩家可将路障放置到当前位置前后10部的任     n为前后相对距离\n");
    printf("                何位置，任一玩家经过将被拦截，一次有效     负数表示后方\n");
    printf("\n");
    printf("Robot           使用该道具将清除前方10步内任何其他道具\n");
    printf("\n");
    printf("Query           显示自家资产\n");
    printf("\n");
    printf("Help            查看命令帮助\n");
    printf("\n");
    printf("Quit            强制退出\n\n");
}


// 逻辑有点冗余，可以修改一下
void func_pass_tool(Game_t* game_ptr,int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    func_print_hint(player_ptr[player_id]->role);
    printf("欢迎光临道具屋，请选择您需要的道具:\n");
    printf("1.路障 所需价值点数%d\n", BARRIER_PRICE);
    printf("2.机器娃娃 所需价值点数为%d\n", ROBOT_PRICE);
    printf("3.炸弹 所需价值点数为%d\n", BOMB_PRICE);
    printf("按'F'退出道具屋\n");
    char c;
    int sum = player_ptr[player_id]->barrier_cnt+player_ptr[player_id]->bomb_cnt+player_ptr[player_id]->robot_cnt;
    while(1)
    {
        if(player_ptr[player_id]->point < ROBOT_PRICE)
        {
          printf("你的点数不够，已退出道具屋\n");
          return ;
        }
        if(sum >= 10)
        {
          printf("你的道具库已经满了,已退出道具屋\n");
          return ;
        }
        c = scanf_char();
        while( c== ' ')
        {
            printf("输入错误，请重新输入\n");
            c = scanf_char();
        }
        if(c=='F'||c=='f')
        {
            printf("你成功退出道具屋\n");
            return ;
        }
        else if(c=='1')
        {
            if(player_ptr[player_id]->point<50)
            {
                printf("你的点数不够，请重新输入\n");
                c = scanf_char();
                while(c==' ')
                {
                    printf("输入错误，请重新输入\n");
                    c = scanf_char();
                }
                if(c=='F'||c=='f')
                {
                    printf("你成功退出道具屋\n");
                    return ;
                }
            }
            else{
                player_ptr[player_id]->point-=50;
                player_ptr[player_id]->barrier_cnt++;
                sum++;
                printf("购买路障成功\n继续购买请再次输入\n");
            }
        }
        else if(c=='3')
        {  
            if(player_ptr[player_id]->point<50)
            {
                printf("你的点数不够，请重新输入\n");
                printf("你的点数不够，请重新输入\n");
                c=scanf_char();
                while(c==' ')
                {
                    printf("输入错误，请重新输入\n");
                    c=scanf_char();
                }
                if(c=='F'||c=='f')
                {
                    printf("你成功退出道具屋\n");
                    return ;
                }
            }
            else{
                player_ptr[player_id]->point-=50;
                player_ptr[player_id]->bomb_cnt++;
                sum++;
                printf("购买炸弹成功\n继续购买请再次输入\n");
            }
        }
        else if(c=='2')
        {
            player_ptr[player_id]->point-=30;
            player_ptr[player_id]->robot_cnt++;
            sum++;
            printf("购买机器人娃娃成功\n继续购买请再次输入\n");
        }
    }
    return;
}


// 礼品屋没提示输出错误
void func_pass_gift(Game_t* game_ptr,int player_id)
{  
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    func_print_hint(player_ptr[player_id]->role);
    printf("欢迎光临礼品屋，请选择一件您喜欢礼品的\n");
    printf("1.奖金\n");
    printf("2.点数卡\n");
    printf("3.财神\n");
    char c;
    c = scanf_char();
    if(c=='1')
    {
        player_ptr[player_id]->money+=2000;
        printf("你成功选择奖金，并且退出礼品屋\n");
    }
    else if(c=='2')
    {
        player_ptr[player_id]->point+=200;
        printf("你成功选择点数卡，并且退出礼品屋\n");
    }
    else if(c=='3')
    {
        player_ptr[player_id]->free_of_toll_cnt+=5;//采用加5;
        printf("你成功选择财神,并且退出礼品屋\n");
    }
    else 
    {
       printf("输入错误，你已经退出礼品屋\n");
    }
    return ;
}

void func_pass_magic(Game_t* game_ptr,int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    printf("你进入到魔法屋，请选择你所需要的魔法\n");
    /*
      待迭代开发；
    */
    return ;
}


/**
 * @brief 
 * 
 * @param game_ptr 
 * @param pos 
 * @return 1 表示是特殊位置，0表示不是特殊位置 
 */
int func_check_special_pos(Game_t* game_ptr, int pos)
{
    if(pos != START_POS && pos != HOSPITAL_POS && pos != TOOL_POS && pos != GIFT_POS && pos != PRISON_POS && pos != MAGIC_POS)
    {
        return 0;
    }
    return 1;
}


/**
 * @brief 
 * 
 * @param game_ptr 
 * @param pos 
 * @return int 1 表示有人在这，0表示没人 
 */
int func_check_some_one_here(Game_t* game_ptr, int pos)
{
    for (int i = 0; i < game_ptr->player_num; ++i)
    {
        if (game_ptr->players_ptr[i]->lose)
        {
            continue;
        }

        if (game_ptr->players_ptr[i]->pos == pos)
        {
            return 1;
        }
    }
    return 0;
}


// for test
// good
void func_set_barrier(Game_t* game_ptr, int barrier_pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    land_ptr[barrier_pos]->item = BARRIER;
    return ;
}

// good
void func_set_unmap(Game_t* game_ptr, int unmap_pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    land_ptr[unmap_pos]->owner_id = -1;
    land_ptr[unmap_pos]->type = VOID_LAND;
    return ;
}

// good
void func_set_bomb(Game_t* game_ptr, int bomb_pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    land_ptr[bomb_pos]->item = BOMB;
    return ;
}

// good，考虑+1
void func_set_stop(Game_t* game_ptr, const char name_char, int stop_time)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    for(int player_id = 0 ; player_id < game_ptr->player_num; player_id++)
    {
       if(role_symbol[player_ptr[player_id]->role] == name_char)
        {
            player_ptr[player_id]->recovery_time_cnt = stop_time;
            return ;
        }
    }
    return ;
}

// good
void func_set_map(Game_t* game_ptr, const char name_char, int map_pos, int level)
{
     Player_t** player_ptr = game_ptr->players_ptr;
     Land_t** land_ptr = game_ptr->land_ptr;
     for(int player_id = 0 ; player_id < game_ptr->player_num; player_id++)
     {
        if(role_symbol[player_ptr[player_id]->role] == name_char)
        {
            land_ptr[map_pos]->owner_id = player_id;
            land_ptr[map_pos]->type = level;
            land_ptr[map_pos]->price = land_ptr[map_pos]->base_price * (level+1);
            return;
        }
     }
     return;
}

// good
void func_set_money(Game_t* game_ptr, char role_char, int money_num)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    for(int player_id = 0; player_id < game_ptr->player_num; player_id++)
    {
        if(role_symbol[player_ptr[player_id]->role] == role_char)
        {
            player_ptr[player_id]->money = money_num;
            return;
        }
    }
     return ;
}

// good , 没考虑+1
void func_set_buff(Game_t* game_ptr, const char name_char,int buff_num)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    for(int player_id = 0 ;player_id < game_ptr->player_num; player_id++)
    {
        if(role_symbol[player_ptr[player_id]->role] == name_char)
        {
            player_ptr[player_id]->free_of_toll_cnt = buff_num;
            return ;
        }
    }
     return ;
}

// good
void func_set_item(Game_t* game_ptr, const char name_char, int item_type, int item_num)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    for(int player_id = 0 ; player_id < game_ptr->player_num; player_id++)
    {
        if(role_symbol[player_ptr[player_id]->role] == name_char)
        {
            if(item_type == BARRIER)
            {
                player_ptr[player_id]->barrier_cnt = item_num;
            }
            else if(item_type == ROBOT)
            {
                player_ptr[player_id]->robot_cnt = item_num;
            }
            else if(item_type == BOMB)
            {
                player_ptr[player_id]->bomb_cnt = item_num;
            }
            return;
        }
    }
    return;
}


void func_set_pos(Game_t* game_ptr, char name_char, int pos_num)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    for(int player_id = 0 ; player_id < game_ptr->player_num; player_id++)
    {
        if(role_symbol[player_ptr[player_id]->role] == name_char)
        {
            player_ptr[player_id]->pos=pos_num;
            return ;
        }
    }
     return;
}

// good
void func_set_point(Game_t* game_ptr, char name_char, int point)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    for(int player_id = 0; player_id < game_ptr->player_num; player_id++)
    {
        if(role_symbol[player_ptr[player_id]->role] == name_char)
        {
            player_ptr[player_id]->point = point;
            return ;
        }
    }
     return ;
}