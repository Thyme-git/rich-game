/**
 * @file display.h
 * @author yyj
 * @brief 显示地图以及人物，以及对应语句的打印
 * @version 0.1
 * @date 2023-07-03
 *
 * @copyright Copyright (c) 2023
 *
 */

# ifndef __DISPLAY_H
# define __DISPLAY_H
# include <stdio.h>
# include "utils.h"
# include "graph.h"

/*
 * brief:根据单个地图块的属性来设置要显示的单个位置
 * param:  land_ptr为要设置的地图块的指针，coord为对应地块的坐标
 * ret: 无
 */
void func_set_pixel_by_land_block(Land_t *land_ptr, int coord);

#endif