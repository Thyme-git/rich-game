#ifndef _LAND_H_
#define _LAND_H_

# include "item.h"

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

typedef struct _Land
{
    int type; // 土地的类型
    int base_price; // 空地基础价格
    int price; // 土地当前的价值，初始化为0，房子每升一次等级 += base_price
    // int diduan; // 土地地段 考虑删除该属性，因为地段提供的价格信息包含在price中
    int point; // 可以在此获取的点数
    int owner_id; // 判断拥有者是谁，-1表示未占用
    Item_enum item; // 道具类型，VOID_ITEM(0)表示没有
    char symbol; // 显示的土地字符
    Color_enum color; // 显示的土地颜色
}Land_t;

void func_init_land(Land_t* land_ptr[]);

#endif