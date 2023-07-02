# ifndef ITEM_H
# define ITEM_H

# define ITEM_NUM 4

typedef enum _Item
{
    VOID // 无道具
,   BARRIER // 路障
,   ROBOT // 机器娃娃
,   BOMB // 炸弹
}Item_enum; // 道具类型

// const int item_price[ITEM_NUM]; // 用于记录道具价格
// const char item_display[ITEM_NUM]; // 用于记录道具的显示方式

typedef enum _Gift
{
    BONUS// 奖金
,   POINT_CARD // 点数卡
,   BLESS // 财神祝福
} Gift_enum;

# endif