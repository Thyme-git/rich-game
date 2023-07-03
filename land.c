# include <malloc.h>
# include "utils.h"

const char land_symbol[LAND_TYPE_NUM] = {'0', '1', '2', '3', 'S', 'T', 'G', 'M', 'H', 'P', '$'};
// 使用方法 land_symbol[VOID_LAND] 取得 '0'

void func_init_land(Land_t* land_ptr[])
{
    // 普通类型 && 分配内存
    for (int i = 0; i < LAND_NUM; ++i)
    {
        land_ptr[i] = (Land_t*)malloc(sizeof(Land_t));

        int price = LAND_4_PRICE;
        if (i != START_POS && i < TOOL_POS){
            price = LAND_1_PRICE;
        }else if (i > TOOL_POS && i < GIFT_POS){
            price = LAND_2_PRICE;
        }else if (i > GIFT_POS && i < MAGIC_POS){
            price = LAND_3_PRICE;
        }

        land_ptr[i]->type = VOID_LAND; // 先预设为空地
        land_ptr[i]->price = 0;
        land_ptr[i]->base_price = price;
        land_ptr[i]->point = 0;
        land_ptr[i]->owner_id = -1; // 没有占用
        land_ptr[i]->item = VOID_ITEM;
    }

    // 初始化特殊类型
    land_ptr[START_POS]->type = START;
    land_ptr[HOSPITAL_POS]->type = HOSPITAL;
    land_ptr[TOOL_POS]->type = TOOL;
    land_ptr[GIFT_POS]->type = GIFT;
    land_ptr[PRISON_POS]->type = PRISON;
    land_ptr[MAGIC_POS]->type = MAGIC;
    
    // 初始化矿地   
    for (int i = MAGIC_POS+1; i < LAND_NUM; ++i)
    {
        land_ptr[i]->type = MINE;
    }

    // 矿地点数
    land_ptr[MAGIC_POS+1]->point = 60;
    land_ptr[MAGIC_POS+2]->point = 80;
    land_ptr[MAGIC_POS+3]->point = 40;
    land_ptr[MAGIC_POS+4]->point = 100;
    land_ptr[MAGIC_POS+5]->point = 80;
    land_ptr[MAGIC_POS+6]->point = 20;
}