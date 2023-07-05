# ifndef UTILS_H
# define UTILS_H

# define SPACE_STR "                           " // 地图中间一行空格
# define COLOR_NUM 5

# define LAND_NUM 70
# define START_POS 0
# define TOOL_POS 28
# define HOSPITAL_POS 14
# define GIFT_POS 35
# define PRISON_POS 49
# define MAGIC_POS 63
# define LAND_1_PRICE 200 
# define LAND_2_PRICE 500
# define LAND_3_PRICE 300
# define LAND_4_PRICE __INT_MAX__
# define LAND_TYPE_NUM 11

# define ROLE_NUM 4 // 角色数量
# define ROLE_VOID (-1) // 角色不存在

# define DEFAULT_INIT_MONEY 10000
# define MAX_INIT_MONEY 50000
# define MIN_INIT_MONEY 1000
# define RECOVERY_TIME 3
# define PRISON_TIME 2
# define ITEM_NUM 4
# define MAX_ITEM_NUM 10

# define GIFT_MONEY 2000
# define GIFT_POINT 200
# define GIFT_BLESS 5

# define BARRIER_PRICE 50
# define ROBOT_PRICE 30
# define BOMB_PRICE 50
# define MIN_ITEM_PRICE (BARRIER_PRICE > ROBOT_PRICE ?          \
    (ROBOT_PRICE > BOMB_PRICE ? BOMB_PRICE : ROBOT_PRICE) :     \
    (BARRIER_PRICE > BOMB_PRICE ? BOMB_PRICE : BARRIER_PRICE)   \
    )

# define REGEX_MAX_LEN 64
# define REGEX_NUM 20

# define INPUT_BUFFER_SIZE 1024

# include <regex.h>
# include <stdio.h>

typedef enum _Color{
	RED
,	GREEN
,	YELLOW
,	BLUE
,	WHITE
}Color_enum;

typedef enum _Land_enum
{
    VOID_LAND // 空地 0
,   HUT // 茅屋 1
,   HOUSE // 洋房 2
,   SKYCRAPER // 摩天楼 3
,   START // 起点 S
,   TOOL // 道具屋 T
,   GIFT // 礼品屋 G
,   MAGIC // 魔法屋 M
,   HOSPITAL // 医院 H
,   PRISON // 监狱 P
,   MINE // 矿地 $
} Land_enum; // nyw 添加

typedef enum _Item
{
    VOID_ITEM // 无道具
,   BARRIER // 路障
,   ROBOT // 机器娃娃
,   BOMB // 炸弹
}Item_enum; // 道具类型

typedef enum _Role {
    ROLE_Q // 钱夫人
,   ROLE_A // 阿土伯
,   ROLE_S // 孙小美
,   ROLE_J // 金贝贝
} Role_enum;

typedef struct _Pri
{
    Role_enum role;
    struct _Pri* next;
}Privilige_t; // 人物优先级排序 Q A S 表示 QAS优先级依次下降,显示时显示优先级最低的。

typedef struct _Land
{
    int type; // 土地的类型
    int base_price; // 空地基础价格
    int price; // 土地当前的价值，初始化为0，房子每升一次等级 += base_price
    int point; // 可以在此获取的点数
    int owner_id; // 判断拥有者是谁，-1表示未占用
    Item_enum item; // 道具类型，VOID_ITEM(0)表示没有
    char symbol; // 显示的土地字符
    Color_enum color; // 显示的土地颜色
    Privilige_t* privilige_role;
}Land_t;

typedef struct _Player
{
    int id; // id 作为玩家的标识，并表示投骰子的顺序
    Role_enum role; // 表示玩家所选的角色
    Color_enum color; // 玩家对应的color
    int pos; // 玩家的坐标，将地图看成一维数组，pos是数组的下标
    int money; // 玩家拥有的金钱数量，用来买地和房子，注意有初始资金
    int point; // 玩家拥有的点数，用来买道具
    int barrier_cnt; // 玩家拥有的路障数量
    int bomb_cnt; // 玩家拥有的炸弹数量
    int robot_cnt; // 玩家拥有的机器娃娃数量
    int free_of_toll_cnt; // 玩家拥有面过路费的次数，当获得财神礼品时刷新为5或加5？
    int recovery_time_cnt; // 玩家被炸伤后剩余的恢复天数，被炸伤后变为3;
    int lose; // 1表示破产输了，0表示沒輸
}Player_t;

typedef struct _Game
{
    Player_t* players_ptr[ROLE_NUM];  // 最多四个人 in player.c
    int player_num; // 玩家数量 [2~4]
    int init_money; // 玩家初始资金，在程序开始时根据输入初始化初始化

    // int curr_player_id; // 当前玩家id，初始化为0;考虑去除，可能用不到

    Land_t* land_ptr[LAND_NUM]; // 游戏的地图
} Game_t;


typedef enum _Gift
{
    BONUS// 奖金
,   POINT_CARD // 点数卡
,   BLESS // 财神祝福
} Gift_enum;

typedef enum _Order
{
    ORDER_ROLL    
,   ORDER_ROBOT   
,   ORDER_QUERY   
,   ORDER_HELP    
,   ORDER_QUIT    
,   ORDER_STEP    
,   ORDER_SELL    
,   ORDER_BLOCK   
,   ORDER_MONEY   
,   ORDER_BUFF    
,   ORDER_POINT   
,   ORDER_STOP    
,   ORDER_POS     
,   ORDER_MAP    
,   ORDER_ITEM     
,   ORDER_UNMAP
,   ORDER_SET_BOMB
,   ORDER_BARRIER
,   ORDER_BOMB
,   ORDER_DUMP
}Order_enum;

typedef struct _Reg
{
    regex_t reg;
    regmatch_t reg_match;
    char regex[REGEX_MAX_LEN];
}Reg_t;


void get_cmd(Game_t* game_ptr, int player_id);

void func_print_hint(Role_enum role);
void func_display_map(Game_t* game_ptr, int player_id);
void func_print_with_color(Color_enum color, char chr);
void func_update_map_info(Game_t* game_ptr, int player_id);

/**
 * @brief
 * 使用方法
 * Game_t* game_ptr = NULL;
 * game_ptr = func_init_game();
 */
Game_t* func_init_game();

void func_init_money(Game_t* game_ptr);

/**
 * @brief 
 * 使用方法
 * int done = func_game_step(game_ptr)
 * 运行一回合，即所有可以动的玩家进行一次投骰子
 * 当返回为1时，游戏结束
 */
int func_game_step(Game_t* game_ptr);

void func_free_mem(Game_t* game_ptr);

void func_game_over(Game_t* game_ptr);

int func_check_game_over(Game_t* game_ptr);

void func_change_pos(Game_t* game_ptr, int player_id, int dst);

void func_check_buy(Land_t* land_ptr, Player_t* player_ptr);

void func_check_update(Land_t* land_ptr, Player_t* player_ptr);

int func_roll(Game_t* game_ptr, int player_id);

int func_player_suffer(Game_t* game_ptr, int player_id, int pos);

void func_player_go_prison(Game_t* game_ptr, int player_id);

void func_step(Game_t* game_ptr, int player_id, int steps);

void func_sell(Game_t* game_ptr, int player_id, int sell_pos);

void func_block(Game_t* game_ptr, int player_id, int block_pos);

void func_bomb(Game_t* game_ptr, int player_id, int bomb_pos);

void func_robot(Game_t* game_ptr,int player_id);

void func_query(Game_t* game_ptr,int player_id);

void func_quit(Game_t* game_ptr);

void func_help();

void func_pay_toll(Game_t* game_ptr, int player_id);

void func_get_point(Game_t* game_ptr, int player_id);

void func_bankrupt(Game_t* game_ptr, int player_id);

void func_pass_tool(Game_t* game_ptr,int player_id);

void func_pass_gift(Game_t* game_ptr,int player_id);

void func_pass_magic(Game_t* game_ptr,int player_id);

int func_check_special_pos(Game_t* game_ptr, int pos);

int func_check_some_one_here(Game_t* game_ptr, int pos);

void get_role_order(Role_enum order[]);

// from here test
void func_set_barrier(Game_t* game_ptr, int barrier_pos);

void func_set_unmap(Game_t* game_ptr, int unmap_pos);

void func_set_bomb(Game_t* game_ptr, int bomb_pos);

void func_set_stop(Game_t* game_ptr, const char name, int stop_time);

void func_set_map(Game_t* game_ptr, const char name, int map_pos, int level);

void func_set_money(Game_t* game_ptr, char role_char, int money_num);

void func_set_buff(Game_t* game_ptr, const char name,int buff_num);

void func_set_item(Game_t* game_ptr, const char name, int item_type, int item_num);

void func_set_pos(Game_t* game_ptr, char name_char,int pos_num);

void func_set_point(Game_t* game_ptr, char name_char, int point);

void func_dump(Game_t *game_ptr,int player_id, FILE *fin);

int func_check_is_player(char c);

int func_check_someone_here(Game_t* game_ptr, int pos);

// int func_player_id_to_symbol
// end

int scanf_num();
int func_check_yes_or_no(const char* hint, Role_enum role);
int func_check_blank_input(char buf[]);
Item_enum func_get_item(Role_enum role);
int func_get_gift(Role_enum role);
void func_scanf_str(char buf[]);
int sizeof_num(int a);
char scanf_char();

void func_init_land(Land_t* land_ptr[]);

Player_t* func_init_player(Role_enum role, int id, int init_money);
int func_count_item_num(Player_t* player_ptr);

Reg_t* func_init_reg(const char* regex);

int func_reg_match(Reg_t* reg_ptr, const char* str);

void func_free_reg(Reg_t* reg_ptr);

void func_init_all_reg();

void func_free_all_reg();

Order_enum func_match_order(const char* str);

Privilige_t* func_init_privilige();
void func_pop(Privilige_t* role_pri_ptr, Role_enum role);
void func_push(Privilige_t* role_pri_ptr, Role_enum role);
Role_enum func_front(Privilige_t* role_pri_ptr);
Role_enum func_tail(Privilige_t* role_pri_ptr);
void func_free_pri(Privilige_t* role_pri_ptr);
void func_get_pri_order(Privilige_t* ptr, char buf[]);

# endif