# ifndef PLAYER_H
# define PLAYER_H

# include "item.h"

# define ROLE_NUM 4 // 角色数量

typedef enum _Role {
    ROLE_Q // 钱夫人
,   ROLE_A // 阿土伯
,   ROLE_S // 孙小美
,   ROLE_J // 金贝贝
} Role_enum;

typedef struct _Player
{
    int id; // id 作为玩家的标识，并表示投骰子的顺序
    Role_enum role; // 表示玩家所选的角色
    int pos; // 玩家的坐标，将地图看成一维数组，pos是数组的下标
    int money; // 玩家拥有的金钱数量，用来买地和房子，注意有初始资金
    int point; // 玩家拥有的点数，用来买道具
    int barrier_cnt; // 玩家拥有的路障数量
    int bomb_cnt; // 玩家拥有的炸弹数量
    int robot_cnt; // 玩家拥有的机器娃娃数量
    int free_of_toll_cnt; // 玩家拥有面过路费的次数，当获得财神礼品时刷新为5或加5？
    int recovery_time_cnt; // 玩家被炸伤后剩余的恢复天数，被炸伤后变为3;
}Player_t;

int func_get_points(Player_t* player_ptr);
int func_get_item_num(Player_t* player_ptr);
int func_sub_points(Player_t*, int);
int func_add_item(Player_t*, Item_enum);

# endif