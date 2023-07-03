/**
 * @file graph.h
 * @author yyj
 * @brief 显示底层逻辑
 * @version 0.1
 * @date 2023-07-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __GRAPH_H
#define __GRAPH_H

#include <stdio.h>

#define MAP_BLOCK_NUM_MAX 70
#define MAP_WIDTH 29
#define MAP_HEIGHT 8

typedef enum {
  COLOR_WHITE,
  COLOR_RED,
  COLOR_YELLOW,
  COLOR_BLUE,
  COLOR_GREEN,
} Color_enum;   //颜色的枚举


typedef struct {
  char symbol;          //符号
  Color_enum color;    //颜色
} Pixel_t;      //像素的属性

/*
 * brief:清空地图，全部用‘ ’,颜色默认白色
 * param:  无   
 * ret: 无
 */
void func_clean_frame_buf();

/*
 * brief:根据坐标来设置对应格子要现实的颜色和符号
 * param:  coord为一维坐标，symbol为显示的符号，color为颜色
 * ret: 无
 */
void func_pixel_set_by_coord(int coord, char symbol, Color_enum color);

/*
 * brief:显示地图
 * param: 无
 * ret: 无
 */
void func_push_frame();

/*
 * brief:在二维数组里设置符号属性
 * param: xy为坐标，symbol为显示的符号，color为颜色
 * ret: 无
 */
void func_pixel_set_by_2D(int x, int y, char symbol, Color_enum color);

/*
 * brief: 打印指定颜色的指定字符
 * param: pixel_ptr为对应点的属性
 * ret: 无
 */
void func_push_pixel(Pixel_t * pixel_ptr);


/*
 * brief: 更改打印颜色
 * param: color为要更改的颜色
 * ret:无
 */
void func_set_printf_color(Color_enum color);

/*
 * brief: 重置打印颜色
 * param: 无
 * ret:无
 */
void func_reset_printf_color();

#endif
