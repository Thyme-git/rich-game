# include <malloc.h>
# include <stdlib.h>
# include <stdlib.h> // system

# include "utils.h"

extern char role_symbol[ROLE_NUM];
extern char info_buf[INPUT_BUFFER_SIZE];

Game_t* func_init_game()
{
    Game_t* game_ptr = (Game_t*)malloc(sizeof(Game_t));
    game_ptr->player_num = 0;
    game_ptr->buff_pos = -1;
    game_ptr->next_buff_time = -1;
    game_ptr->round_num = 0;
    // game_ptr->round_num = 10; // for test

    // 输入玩家初始基金
    func_init_money(game_ptr);

    // 初始化Land
    func_init_land(game_ptr->land_ptr);

    // 输入角色
    Role_enum order[ROLE_NUM];
    get_role_order(order);
    for (int i = 0; i < ROLE_NUM; ++i)
    {
        if (order[i] == ROLE_VOID)
        {
            break;
        }
        func_push(game_ptr->land_ptr[START_POS]->privilige_role, order[i]);
        game_ptr->players_ptr[i] = func_init_player(order[i], i, game_ptr->init_money);
        game_ptr->player_num += 1;
    }

    return game_ptr;
}

void func_init_money(Game_t* game_ptr)
{
    game_ptr->init_money = DEFAULT_INIT_MONEY;
    printf("输入玩家初始资金[%d~%d](默认 %d) 或者输入回车跳过:", MIN_INIT_MONEY, MAX_INIT_MONEY, DEFAULT_INIT_MONEY);
    int input_money;
HERE:
    input_money = scanf_num();
    if (input_money == -1) // 直接输入了回车
    {
        printf("资金初始化为默认值 %d\n", DEFAULT_INIT_MONEY);
    }
    else if (input_money < MIN_INIT_MONEY || input_money > MAX_INIT_MONEY)
    {
        printf("输入范围不正确或错误，请重新输入:");
        goto HERE;
    }else{
        game_ptr->init_money = input_money;
        printf("资金初始化为 %d\n", input_money);
    }
}

int func_game_step(Game_t* game_ptr)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    Land_t** land_ptr = game_ptr->land_ptr;

    if (game_ptr->buff_pos == -1 && game_ptr->next_buff_time == -1 && game_ptr->round_num >= BUFF_OCCUR_TIME)
    {
        game_ptr->next_buff_time = game_ptr->round_num + rand() % BUFF_OCCUR_INTERVAL;
    }

    if (game_ptr->buff_pos == -1 && game_ptr->next_buff_time == game_ptr->round_num)
    {
        game_ptr->buff_pos = func_generate_buff(game_ptr);
        game_ptr->buff_keep_time = 0;
        func_concat_info("财神祝福出现了");
    }

    for (int player_id = 0; player_id < game_ptr->player_num; ++player_id)
    {  
        // 判断game over
        if (func_check_game_over(game_ptr))
        {
            func_game_over(game_ptr);
            return 1;
        }

        // 玩家已经输了则不执行操作
        if (player_ptr[player_id]->lose){
            continue;
        }
        
        // 还在医院则不执行操作
        if (player_ptr[player_id]->recovery_time_cnt > 0){
            player_ptr[player_id]->recovery_time_cnt -= 1; 
            continue;
        }

        // 移动前的位置
        int pre_pos = player_ptr[player_id]->pos;

        // 显示地图
        func_display_with_info(game_ptr, player_id, info_buf);

        // 读取命令并判断是否有错误、执行
        get_cmd(game_ptr, player_id);

        // 获取摇骰子后的位置
        int pos = player_ptr[player_id]->pos;
        int land_type = land_ptr[pos]->type;

        // 提示移动的步数
        char buf[INPUT_BUFFER_SIZE];
        sprintf(buf, "%c移动了%d步", func_get_player_symbol(game_ptr, player_id), (pos+LAND_NUM-pre_pos)%LAND_NUM);
        func_concat_info(buf);
        
        func_display_with_info(game_ptr, player_id, info_buf);

        // 提示购买或升级
        func_check_buy_update(game_ptr, player_id);

        // 道具屋
        if (pos == TOOL_POS)
        {
            func_pass_tool(game_ptr, player_id);
        }

        // 魔法屋park
        if (pos == MAGIC_PARK_POS)
        {
            func_pass_park(game_ptr, player_id);
        }


        // 坐牢park
        if (pos == PRISON_PARK_POS)
        {
            func_pass_park(game_ptr, player_id);
        }

        // 医院park
        if (pos == HOSPITAL_PARK_POS)
        {
            func_pass_park(game_ptr, player_id);
        }

        // 禮品屋
        if (pos == GIFT_POS)
        {
            func_pass_gift(game_ptr, player_id);
        }

        // 礦山
        func_get_point(game_ptr, player_id);
        

        // 收租啦
        func_pay_toll(game_ptr, player_id);

        if (game_ptr->players_ptr[player_id]->free_of_toll_cnt > 0)
        {
            game_ptr->players_ptr[player_id]->free_of_toll_cnt -= 1;
        }
    }

    if (game_ptr->buff_pos >= 0)
    {   
        game_ptr->buff_keep_time += 1;
        if (game_ptr->buff_keep_time >= BUFF_MAX_KEEP_TIME)
        {
            game_ptr->land_ptr[game_ptr->buff_pos]->item = VOID_ITEM;
            game_ptr->buff_pos = -1;
            game_ptr->next_buff_time = -1;
        }
    }
    game_ptr->round_num += 1;
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
        func_free_pri(game_ptr->land_ptr[i]->privilige_role);
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
    printf("玩家%c获胜啦！\n", func_get_player_symbol(game_ptr, winner_id));

    extern FILE* dump_fp;

    // dump info
    func_dump(game_ptr, winner_id, dump_fp);

    // 释放内存
    func_free_mem(game_ptr);
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

void func_check_buy(Game_t* game_ptr, int player_id, int pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;

    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "是否购买该空地？（价格：%d）", land_ptr[pos]->base_price);

    if (func_check_yes_or_no(buf, player_ptr[player_id]->role))
    {
        if (player_ptr[player_id]->money >= land_ptr[pos]->base_price)
        {
            player_ptr[player_id]->money -= land_ptr[pos]->base_price;
            player_ptr[player_id]->solid_property_cnt[VOID_LAND] += 1;
            land_ptr[pos]->price += land_ptr[pos]->base_price;
            land_ptr[pos]->owner_id = player_ptr[player_id]->id;
            func_concat_info("购买成功！");
        }else{
            func_concat_info("资金不足无法购买！");
        }
    }
}

void func_check_update(Game_t* game_ptr, int player_id, int pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;

    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "是否升级该房产？（价格：%d）", land_ptr[pos]->base_price);

    if (func_check_yes_or_no(buf, player_ptr[player_id]->role))
    {
        if (player_ptr[player_id]->money >= land_ptr[pos]->base_price)
        {
            player_ptr[player_id]->money -= land_ptr[pos]->base_price;
            land_ptr[pos]->price += land_ptr[pos]->base_price;
            player_ptr[player_id]->solid_property_cnt[land_ptr[pos]->type] -= 1;
            player_ptr[player_id]->solid_property_cnt[land_ptr[pos]->type+1] += 1;
            land_ptr[pos]->type += 1;
            func_concat_info("升级成功！");
        }else{
            func_concat_info("资金不足无法升级！");
        }
    }
}

void func_check_buy_update(Game_t* game_ptr, int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;

    int pos = player_ptr[player_id]->pos;
    int land_type = land_ptr[pos]->type;

    // 提示购买或升级
    if (land_type == VOID_LAND && land_ptr[pos]->owner_id == -1)
    {
        func_check_buy(game_ptr, player_id, pos);
        return;  
    }
    if ((land_type == VOID_LAND || land_type == HUT || land_type == HOUSE) && land_ptr[pos]->owner_id == player_id)
    {
        func_check_update(game_ptr, player_id, pos);
        return;
    }

}

int func_roll(Game_t* game_ptr, int player_id)
{
    int steps = rand()%6+1;
    func_step(game_ptr, player_id, steps);
}

/**
 * @brief 清除路上的路障并将玩家移动到路障处
 * @param game_ptr 
 * @param pos 
 */
void func_suffer_barrier(Game_t* game_ptr, int player_id, int pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;

    func_change_pos(game_ptr, player_id, pos);
    land_ptr[pos]->item = VOID_ITEM;

    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "此路不通，%c被拦下来辽", func_get_player_symbol(game_ptr, player_id));
    func_concat_info(buf);
}

/**
 * @brief 将玩家的位置移动到医院并设置恢复时间,同时清除路上炸弹
 * @param game_ptr 
 * @param player_id 
 * @param pos 
 */
// void func_suffer_bomb(Game_t* game_ptr, int player_id, int pos)
// {
//     Land_t** land_ptr = game_ptr->land_ptr;
//     Player_t** player_ptr = game_ptr->players_ptr;
    
//     func_change_pos(game_ptr, player_id, HOSPITAL_POS);
//     player_ptr[player_id]->recovery_time_cnt = RECOVERY_TIME;
//     land_ptr[pos]->item = VOID_ITEM;
//     sprintf(info_buf, "被炸伤啦，送往医院！");
// }

// void func_player_go_prison(Game_t* game_ptr, int player_id)
// {
//     Player_t** player_ptr = game_ptr->players_ptr;
//     player_ptr[player_id]->recovery_time_cnt = PRISON_TIME;
// }

void func_step(Game_t* game_ptr, int player_id, int steps)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;

    int start_pos = (player_ptr[player_id]->pos + 1) % LAND_NUM;
    
    for (int i = 0; i < steps; ++i)
    {
        // 财神祝福
        if (game_ptr->land_ptr[(i+start_pos) % LAND_NUM]->item == BUFF)
        {
            func_get_buff(game_ptr, player_id, (i+start_pos) % LAND_NUM);
        }
        
        // 遇到路障
        if (land_ptr[(i+start_pos) % LAND_NUM]->item == BARRIER)
        {
            func_suffer_barrier(game_ptr, player_id, (i+start_pos) % LAND_NUM);
            return;
        }
    }

    func_change_pos(game_ptr, player_id, (player_ptr[player_id]->pos+steps) % LAND_NUM);
}

void func_get_buff(Game_t* game_ptr, int player_id, int pos)
{
    game_ptr->players_ptr[player_id]->free_of_toll_cnt = GIFT_BLESS;
    game_ptr->buff_keep_time = 0;
    game_ptr->land_ptr[game_ptr->buff_pos]->item = VOID_ITEM;\
    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "%c获得财神祝福", func_get_player_symbol(game_ptr, player_id));
    func_concat_info(buf);
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
    char buf[INPUT_BUFFER_SIZE];

    // 土地沒有主人 或者 主人是自己 或者 主人在監獄中
    if (owner_id == -1 || owner_id == player_id || game_ptr->players_ptr[owner_id]->recovery_time_cnt > 0)
    {
        return;
    }

    // 財神祝福
    if (game_ptr->players_ptr[player_id]->free_of_toll_cnt > 0)
    {
        sprintf(buf, "%c財神附身，可免過路費", func_get_player_symbol(game_ptr, player_id));
        func_concat_info(buf);
        return;
    }

    game_ptr->players_ptr[player_id]->money -= game_ptr->land_ptr[pos]->price / 2;
    game_ptr->players_ptr[owner_id]->money += game_ptr->land_ptr[pos]->price / 2;

    buf[INPUT_BUFFER_SIZE];
    // 破產
    if (game_ptr->players_ptr[player_id]->money < 0)
    {
        game_ptr->players_ptr[owner_id]->money -= game_ptr->land_ptr[pos]->price / 2;
        func_bankrupt(game_ptr, player_id);
        sprintf(buf, "%c破产辣", func_get_player_symbol(game_ptr, player_id));
        func_concat_info(buf);
    }else{
        sprintf(buf, "%c被%c收取了%d租金", func_get_player_symbol(game_ptr, player_id), func_get_player_symbol(game_ptr, owner_id), game_ptr->land_ptr[pos]->price / 2);
        func_concat_info(buf);
    }
}

void func_bankrupt(Game_t* game_ptr, int player_id)
{
    game_ptr->players_ptr[player_id]->lose = 1;
    game_ptr->players_ptr[player_id]->money = 0;
    Land_t** land_ptr = game_ptr->land_ptr;
    func_pop(land_ptr[game_ptr->players_ptr[player_id]->pos]->privilige_role, game_ptr->players_ptr[player_id]->role);
    for (int i = 0; i < LAND_NUM; ++i)
    {
        if (land_ptr[i]->owner_id == player_id)
        {
            land_ptr[i]->owner_id = -1;
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
    player_ptr[player_id]->solid_property_cnt[land_ptr[sell_pos]->type] -= 1;
    land_ptr[sell_pos]->type = VOID_LAND;
    land_ptr[sell_pos]->owner_id = -1;

    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "%c售卖成功！获得资金：%d", func_get_player_symbol(game_ptr, player_id), 2*land_ptr[sell_pos]->price);
    func_display_with_info(game_ptr, player_id, buf);
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

    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "%c投放路障成功", func_get_player_symbol(game_ptr, player_id));
    func_display_with_info(game_ptr, player_id, buf);
}

// void func_bomb(Game_t* game_ptr, int player_id, int bomb_pos)
// {
//     Land_t** land_ptr = game_ptr->land_ptr;
//     Player_t** player_ptr = game_ptr->players_ptr;
//     int pos = player_ptr[player_id]->pos;

//     if (player_ptr[player_id]->bomb_cnt <= 0)
//     {
//         printf("你没有炸弹可以投放！\n");
//         return;
//     }

//     if (bomb_pos < -10 || bomb_pos > 10)
//     {
//         printf("投放的距离太远！\n");
//         return;
//     }

//     int target_pos = (pos + bomb_pos) % LAND_NUM;
//     if (target_pos < 0)
//     {
//         target_pos += LAND_NUM;
//     }

//     // 特殊位置不能放
//     if (func_check_special_pos(game_ptr, target_pos))
//     {
//         printf("特殊位置不能投放！\n");
//         return;
//     }

//     // 有人不能放炸弹
//     if (func_check_some_one_here(game_ptr, target_pos))
//     {
//         printf("有玩家的位置不能投放！\n");
//         return;
//     }

//     if (land_ptr[target_pos]->item != VOID_ITEM)
//     {
//         printf("此处已经有道具，不能再次放！\n");
//         return;
//     }

//     player_ptr[player_id]->bomb_cnt -= 1;
//     land_ptr[target_pos]->item = BOMB;

//     func_display_with_info(game_ptr, player_id, "投放成功！");
// }

void func_robot(Game_t* game_ptr,int player_id)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    
    int pos = player_ptr[player_id]->pos;
    if ( player_ptr[player_id]->robot_cnt <= 0)
    {
        printf("你没有机器人可以使用！\n");
        return;
    }

    for (int i = 1; i <= 10;i++)
    {
        land_ptr[(pos+i) % LAND_NUM]->item=VOID_ITEM;
    }

    player_ptr[player_id]->robot_cnt -= 1;

    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "%c使用机器娃娃成功", func_get_player_symbol(game_ptr, player_id));
    func_display_with_info(game_ptr, player_id, buf); 
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
    // printf("炸弹:%d\n", player_ptr[player_id]->bomb_cnt);
    printf("机器娃娃:%d\n", player_ptr[player_id]->robot_cnt);
    printf("空地数量:%d", player_ptr[player_id]->solid_property_cnt[VOID_LAND]);
    func_print_house_pos(game_ptr, player_id, VOID_LAND);
    printf("茅屋数量:%d", player_ptr[player_id]->solid_property_cnt[HUT]);
    func_print_house_pos(game_ptr, player_id, HUT);
    printf("房子数量:%d", player_ptr[player_id]->solid_property_cnt[HOUSE]);
    func_print_house_pos(game_ptr, player_id, HOUSE);
    printf("摩天楼数量:%d", player_ptr[player_id]->solid_property_cnt[SKYCRAPER]);
    func_print_house_pos(game_ptr, player_id, SKYCRAPER);
    // printf("\n");
}

/**
 * @brief 获取房产的位置
 * 
 * @param pos 
 */
void func_print_house_pos(Game_t* game_ptr, int player_id, int type)
{
    int cnt = 0;
    int pos[LAND_NUM];
    for (int i = 0; i < LAND_NUM; ++i){
        if (game_ptr->land_ptr[i]->owner_id == player_id && game_ptr->land_ptr[i]->type == type)
        {
            pos[cnt++] = i;
        }
    }
    if (cnt == 0)
    {
        putchar('\n');
        return;
    }
    printf("，位于");
    for (int i = 0; i < cnt; ++i)
    {
        printf(" [%d]", pos[i]);
    }
    printf("处\n");
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


void func_pass_tool(Game_t* game_ptr,int player_id)
{
    int done = 0;
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    printf("欢迎光临道具屋，请选择您需要的道具:\n");
    printf("1.路障 所需价值点数%d\n", BARRIER_PRICE);
    printf("2.机器娃娃 所需价值点数为%d\n", ROBOT_PRICE);
    // printf("3.炸弹 所需价值点数为%d\n", BOMB_PRICE);
    printf("按'F'退出道具屋\n");
    // func_print_hint(player_ptr[player_id]->role);

    char buf[INPUT_BUFFER_SIZE];

    if (player_ptr[player_id]->point < MIN_ITEM_PRICE)
    {
        sprintf(buf, "%c点数少于最少点数的道具，退出道具屋！", func_get_player_symbol(game_ptr, player_id));
        func_concat_info(buf);
        return;
    }

    while (!done)
    {
        Item_enum choice = func_get_item(player_ptr[player_id]->role);
        if (choice == VOID_ITEM){
            done = 1;
        }else if (func_count_item_num(player_ptr[player_id]) >= MAX_ITEM_NUM){
            printf("背包已经满了（%d/%d），不能再购买道具！\n", MAX_ITEM_NUM, MAX_ITEM_NUM);
        }else if (choice == BARRIER){
            if (player_ptr[player_id]->point < BARRIER_PRICE)
            {
                printf("你的点数不足，请重新选择！\n");
            }else{
                player_ptr[player_id]->point -= BARRIER_PRICE;
                player_ptr[player_id]->barrier_cnt += 1;
                printf("购买成功！\n");
            }
        }else if (choice == ROBOT){
            if (player_ptr[player_id]->point < ROBOT_PRICE)
            {
                printf("你的点数不足，请重新选择！\n");
            }else{
                player_ptr[player_id]->point -= ROBOT_PRICE;
                player_ptr[player_id]->robot_cnt += 1;
                printf("购买成功！\n");
            }
        }
        // else if (choice == BOMB){
        //     if (player_ptr[player_id]->point < BOMB_PRICE)
        //     {
        //         printf("你的点数不足，请重新选择！\n");
        //     }else{
        //         player_ptr[player_id]->point -= BOMB_PRICE;
        //         player_ptr[player_id]->bomb_cnt += 1;
        //         printf("购买成功！\n");
        //     }
        // }
    }
    return;
}


void func_pass_gift(Game_t* game_ptr,int player_id)
{  
    Land_t** land_ptr = game_ptr->land_ptr;
    Player_t** player_ptr = game_ptr->players_ptr;
    func_print_hint(player_ptr[player_id]->role);
    printf("欢迎光临礼品屋，请选择一件您喜欢礼品的\n");
    printf("1.奖金\n");
    printf("2.点数卡\n");
    // printf("3.财神\n");
    char buf[INPUT_BUFFER_SIZE];
    switch (func_get_gift(player_ptr[player_id]->role))
    {
    case -1:
        sprintf(buf, "%c输入错误，放弃选择！", func_get_player_symbol(game_ptr, player_id));
        func_concat_info(buf);
        break;
    case 1:
        sprintf(buf, "%c获得奖金%d!", func_get_player_symbol(game_ptr, player_id), GIFT_MONEY);
        func_concat_info(buf);
        player_ptr[player_id]->money += GIFT_MONEY;
        break;
    case 2:
        sprintf(buf, "%c获得点数%d!", func_get_player_symbol(game_ptr, player_id), GIFT_POINT);
        func_concat_info(buf);
        player_ptr[player_id]->point += GIFT_POINT;
        break;
    // case 3:
    //     sprintf(info_buf, "获得财神祝福%d回合!", GIFT_BLESS);
    //     player_ptr[player_id]->free_of_toll_cnt = GIFT_BLESS;
    //     break;
    default:
        sprintf(buf, "%c输入错误，放弃选择！", func_get_player_symbol(game_ptr, player_id));
        func_concat_info(buf);
        break;
    }
    return ;
}


// void func_pass_magic(Game_t* game_ptr,int player_id)
// {
//     Land_t** land_ptr = game_ptr->land_ptr;
//     Player_t** player_ptr = game_ptr->players_ptr;
//     printf("你进入到魔法屋，请选择你所需要的魔法\n");
//     /*
//       待迭代开发；
//     */
//     return ;
// }

void func_pass_park(Game_t* game_ptr,int player_id)
{
    char buf[INPUT_BUFFER_SIZE];
    sprintf(buf, "欢迎%c来到公园", func_get_player_symbol(game_ptr, player_id));
    func_concat_info(buf);
}

/**
 * @brief 
 * 
 * @param game_ptr 
 * @param pos 
 * @return 1 表示是特殊位置，0表示不是特殊位置 
 */
// int func_check_special_pos(Game_t* game_ptr, int pos)
// {
//     if(pos != START_POS && pos != HOSPITAL_POS && pos != TOOL_POS && pos != GIFT_POS && pos != PRISON_POS && pos != MAGIC_POS)
//     {
//         return 0;
//     }
//     return 1;
// }


/**
 * @brief 
 * 
 * @param game_ptr 
 * @return int 财神buff的出现的位置
 */
int func_generate_buff(Game_t* game_ptr)
{
    int pos = rand() % LAND_NUM;
    while (!func_check_buff_valid_pos(game_ptr, pos))
    {
        pos = rand() % LAND_NUM;
    }
    func_put_buff(game_ptr, pos);
    return pos;
}

void func_put_buff(Game_t* game_ptr, int pos)
{
    game_ptr->land_ptr[pos]->item = BUFF;
}

/**
 * @brief 
 * 
 * @return int 1表示可以放财神buff,0表示不行
 */
int func_check_buff_valid_pos(Game_t* game_ptr, int pos)
{
    if (func_check_some_one_here(game_ptr, pos) || game_ptr->land_ptr[pos]->item != VOID_ITEM || pos == GIFT_POS || pos == TOOL_POS)
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
    return game_ptr->land_ptr[pos]->privilige_role->next != NULL;
}

void func_change_pos(Game_t* game_ptr, int player_id, int dst)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    Land_t** land_ptr = game_ptr->land_ptr;
    Role_enum role = player_ptr[player_id]->role;
    func_pop(land_ptr[player_ptr[player_id]->pos]->privilige_role, role);
    func_push(land_ptr[dst % LAND_NUM]->privilige_role, role);

    // char buf[INPUT_BUFFER_SIZE];
    // sprintf(buf, "移动了%d步", (dst+LAND_NUM-player_ptr[player_id]->pos)%LAND_NUM);
    player_ptr[player_id]->pos = dst % LAND_NUM;
    // func_display_with_info(game_ptr, player_id, buf);
}

// for test
void func_set_barrier(Game_t* game_ptr, int barrier_pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    land_ptr[barrier_pos]->item = BARRIER;
    return ;
}

void func_set_unmap(Game_t* game_ptr, int unmap_pos)
{
    Land_t** land_ptr = game_ptr->land_ptr;
    game_ptr->players_ptr[land_ptr[unmap_pos]->owner_id]->solid_property_cnt[land_ptr[unmap_pos]->type] -= 1;
    land_ptr[unmap_pos]->owner_id = -1;
    land_ptr[unmap_pos]->type = VOID_LAND;
    return ;
}

// void func_set_bomb(Game_t* game_ptr, int bomb_pos)
// {
//     Land_t** land_ptr = game_ptr->land_ptr;
//     land_ptr[bomb_pos]->item = BOMB;
//     return ;
// }

// void func_set_stop(Game_t* game_ptr, const char name_char, int stop_time)
// {
//     Player_t** player_ptr = game_ptr->players_ptr;
//     for(int player_id = 0 ; player_id < game_ptr->player_num; player_id++)
//     {
//        if(role_symbol[player_ptr[player_id]->role] == name_char)
//         {
//             player_ptr[player_id]->recovery_time_cnt = stop_time;
//             return ;
//         }
//     }
//     return ;
// }

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
            player_ptr[player_id]->solid_property_cnt[level] += 1;
            land_ptr[map_pos]->price = land_ptr[map_pos]->base_price * (level+1);
            return;
        }
     }
     return;
}

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
            // else if(item_type == BOMB)
            // {
            //     player_ptr[player_id]->bomb_cnt = item_num;
            // }
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
            func_change_pos(game_ptr, player_id,  pos_num);
            return ;
        }
    }
    return;
}

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

void func_round(Game_t* game_ptr, int round)
{
    game_ptr->round_num = round;   
}

char func_get_player_symbol(Game_t* game_ptr, int player_id)
{
    return role_symbol[game_ptr->players_ptr[player_id]->role];
}