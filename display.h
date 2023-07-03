#ifndef _OUT_H_
#define _OUT_H_
#include <stdio.h>
# include"land.h"
# include"display.h"
# include "player.h"
# include "game.h"

# define SPACE_STR "                           " // 地图中间一行空格
# define COLOR_NUM 5

typedef enum _Color{
	RED
,	GREEN
,	YELLOW
,	BLUE
,	WHITE
}Color_enum;

// const char* color_table[COLOR_NUM] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[0m"}; // 考虑移动到display.c中定义
// 使用方法：color_table[RED]获取字符串"\033[31m"


//打印地图的接口
void func_print_hint(Role_enum role)
void func_display_map(Game_t*);
void func_print_with_color(Color_enum color, char chr);
void func_print_help();

#endif